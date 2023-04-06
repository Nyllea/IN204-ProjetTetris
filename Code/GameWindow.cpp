#include "GameWindow.hpp"

void SetupGrid(Gtk::Grid *grid, const guint spacing)
{
	// Initialisation de la grille

	grid->set_row_homogeneous(false);
	grid->set_column_homogeneous(false);

	grid->set_column_spacing(spacing);
	grid->set_row_spacing(spacing);

	grid->set_hexpand(true);
	grid->set_halign(Gtk::Align::ALIGN_CENTER);
	grid->set_vexpand(true);
	grid->set_valign(Gtk::Align::ALIGN_CENTER);
}

GameWindow::GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing)
	: gameOverMenu(NULL), bestScore(0), currentMainLoopTimeout(MAIN_LOOP_TIMEOUT), pauseMenu(NULL)
{
	// On est pas encore dans le jeu
	isInGame = false;

	// Paramétrage de la fenetre
	set_title(name);
	set_default_size(width, height);
	set_resizable(false);
	set_border_width(borderSize);
	set_position(Gtk::WIN_POS_CENTER);
	override_background_color(Gdk::RGBA(BACKGROUND_COLOR), Gtk::StateFlags::STATE_FLAG_NORMAL);

	// Initialisation du provider
	cssProvider = Gtk::CssProvider::create();
	cssProvider->load_from_path("Data.css");
	Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	// Génération des classes de jeu
	terrainPiece.terrainGraph = new TerrainGraphic();
	terrainPiece.previewGraph = new PreviewGraphic();
	terrainPiece.previousPreviewGraph = new PreviewGraphic();

	// Génération des pièces futures et assignation d'une zone mémoire pour la piece actuelle
	timeManager.GeneratePieces();
	terrainPiece.pieceGraph = new (PieceGraphic *)(timeManager.GetNextPiece());

	// Initialisation de la grille de jeu
	Gtk::Grid *m_terrainGrid = Gtk::make_managed<Gtk::Grid>(); // Laisse Gtk delete la grille quand la fenetre se ferme
	SetupGrid(m_terrainGrid, gridSpacing);

	terrainPiece.terrainGraph->SetGrid(m_terrainGrid);
	terrainPiece.terrainGraph->FillGrid(width, height);

	// Initialisation de la prévisualisation de la pièce
	Gtk::Grid *m_previewGrid = Gtk::make_managed<Gtk::Grid>();
	SetupGrid(m_previewGrid, gridSpacing);

	terrainPiece.previewGraph->SetGrid(m_previewGrid);
	terrainPiece.previewGraph->FillGrid(width, height);

	// Initialisation de la prévisualisation de la pièce du passé
	Gtk::Grid *m_previousPreviewGrid = Gtk::make_managed<Gtk::Grid>();
	SetupGrid(m_previousPreviewGrid, gridSpacing);

	terrainPiece.previousPreviewGraph->SetGrid(m_previousPreviewGrid);
	terrainPiece.previousPreviewGraph->FillGrid(width, height);

	// Initialisation des visualisation des score, meilleur score et niveau
	scoreLabel = Gtk::make_managed<Gtk::Label>();
	bestScoreLabelPauseMenu = Gtk::make_managed<Gtk::Label>();
	levelLabel = Gtk::make_managed<Gtk::Label>();
	scoreLabelOverMenu = Gtk::make_managed<Gtk::Label>();
	bestScoreLabelOverMenu = Gtk::make_managed<Gtk::Label>();
	bestScoreLabelMainMenu = Gtk::make_managed<Gtk::Label>();

	bestScoreLabelMainMenu->get_style_context()->add_class("subtitleLabel");
	scoreLabelOverMenu->get_style_context()->add_class("subtitleLabelMenu");
	bestScoreLabelOverMenu->get_style_context()->add_class("subtitleLabelMenu");
	levelLabel->get_style_context()->add_class("subtitleLabelMenu");
	bestScoreLabelPauseMenu->get_style_context()->add_class("subtitleLabelMenu");
	scoreLabel->get_style_context()->add_class("subtitleLabelMenu");

	// Initialise l'affichage des pastilles de temps
	ActualiseTimeStickers();
	for (int i = 0; i < MAX_PREDICTION; i++)
	{
		timeLabels[i].set_text("oo");
	}

	// Initialisation de l'affichage des scores
	RenderScore(0);
	RenderScore(1);

	// Création du terrain de jeu
	gameBoard = MakeGameBoard(m_terrainGrid, m_previewGrid, m_previousPreviewGrid);

	// Création de l'overlay
	overlay = Gtk::make_managed<Gtk::Overlay>();
	add(*overlay); // Ajout de l'overlay à la fenêtre

	// Ajout du menu principal à l'overlay
	mainMenu = MakeMainMenu();
	overlay->add_overlay(*mainMenu);

	// Gestion des entrées clavier
	add_events(Gdk::KEY_PRESS_MASK);
	keyboardControls = signal_key_press_event().connect(sigc::mem_fun(*this, &GameWindow::OnKeyPress), false);
	keyboardControls.block();
}

GameWindow::~GameWindow()
{
	keyboardControls.disconnect();
	mainGameLoop.disconnect();

	delete *terrainPiece.pieceGraph;
	delete terrainPiece.pieceGraph;
	delete terrainPiece.previousPreviewGraph;
	delete terrainPiece.previewGraph;
	delete terrainPiece.terrainGraph;
}

Gtk::Box *GameWindow::MakeGameBoard(Gtk::Grid *terrainGrid, Gtk::Grid *previewGrid, Gtk::Grid *previousPreviewGrid)
{
	// Création des widgets nécessaires
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
	Gtk::Box *leftWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Box *rightWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Button *pauseBtn = Gtk::make_managed<Gtk::Button>("Pause");
	Gtk::Box *smallWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

	// Connecte le bouton aux actions à réaliser
	pauseBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::PauseButton));

	// Style avec CSS
	wrapper->get_style_context()->add_class("mainMenuBackgroud");
	pauseBtn->get_style_context()->add_class("button");
	for (int i = 0; i < MAX_PREDICTION; i++)
	{
		smallWrapper->pack_start(timeLabels[i], Gtk::PACK_SHRINK, 0);
	}

	// Rassemblements des widgets à gauche du terrain
	leftWrapper->set_homogeneous(true);
	leftWrapper->set_hexpand(false);
	leftWrapper->pack_start(*scoreLabel, Gtk::PACK_SHRINK, 0);
	leftWrapper->pack_start(*pauseBtn, Gtk::PACK_SHRINK, 0);
	leftWrapper->pack_start(*previousPreviewGrid, Gtk::PACK_SHRINK, 0);

	// Rassemblements des widgets à gauche du terrain
	rightWrapper->set_homogeneous(true);
	rightWrapper->set_hexpand(false);
	rightWrapper->pack_start(*smallWrapper, Gtk::PACK_SHRINK, 0);
	rightWrapper->pack_start(*levelLabel, Gtk::PACK_SHRINK, 0);
	rightWrapper->pack_start(*previewGrid, Gtk::PACK_SHRINK, 0);

	// Paramètres de la boite principale
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);

	// Ajouter les widget à la boite principale
	wrapper->pack_start(*leftWrapper, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*terrainGrid, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*rightWrapper, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

Gtk::Box *GameWindow::MakeMainMenu()
{
	// Création des widgets nécessaires
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *mainMenuLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *startBtn = Gtk::make_managed<Gtk::Button>("Start Game");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Initialisation des textes
	mainMenuLabel->set_text("TIME TETRIS");

	// Connecte les boutons aux actions à réaliser
	startBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::StartButton));
	exitBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ExitGame));

	// Style avec CSS
	wrapper->get_style_context()->add_class("mainMenuBackgroud");
	mainMenuLabel->get_style_context()->add_class("titleLabel");
	startBtn->get_style_context()->add_class("button");
	exitBtn->get_style_context()->add_class("button");

	// Paramètrage de la boite principale
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	wrapper->set_margin_bottom(100);

	// Ajouter les widget au wrapper
	wrapper->pack_start(*mainMenuLabel, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*bestScoreLabelMainMenu, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*startBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*exitBtn, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

Gtk::Box *GameWindow::MakeGameOverMenu()
{
	// Création des widgets nécessaires
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *gameOverLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Box *scoreBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
	Gtk::Label *locScoreLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *retryBtn = Gtk::make_managed<Gtk::Button>("Retry");
	Gtk::Button *mainMenuBtn = Gtk::make_managed<Gtk::Button>("Main menu");

	// Initialisation des textes
	gameOverLabel->set_text("GAME OVER");

	// Connecte les boutons aux actions à réaliser
	retryBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::RestartGame));
	mainMenuBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::MainMenuButton));

	// Style avec CSS
	retryBtn->get_style_context()->add_class("button");
	mainMenuBtn->get_style_context()->add_class("button");
	gameOverLabel->get_style_context()->add_class("GOtitleLabel");

	// Paramètrage de la boîte principale
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	wrapper->set_vexpand(false);

	// Paramètrage de la sous-boite contenant les scores
	scoreBox->set_homogeneous(true);
	scoreBox->set_hexpand(false);

	// Ajouter les widget au wrapper
	wrapper->pack_start(*gameOverLabel, Gtk::PACK_SHRINK, 0);
	scoreBox->pack_start(*scoreLabelOverMenu, Gtk::PACK_SHRINK, 0);
	scoreBox->pack_start(*bestScoreLabelOverMenu, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*scoreBox, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*retryBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*mainMenuBtn, Gtk::PACK_SHRINK, 0);
	wrapper->override_background_color(Gdk::RGBA(GAMEOVER_BKG_COLOR), Gtk::STATE_FLAG_NORMAL);

	return wrapper;
}

Gtk::Box *GameWindow::MakePauseMenu()
{
	// Création des widgets nécessaires
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Box *scoreBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
	Gtk::Label *locScoreLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *resumeBtn = Gtk::make_managed<Gtk::Button>("Resume");
	Gtk::Button *restartBtn = Gtk::make_managed<Gtk::Button>("Restart");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Connecte les boutons aux actions à réaliser
	resumeBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ResumeButton));
	restartBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::RestartGame));
	exitBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ExitGame));

	// Style avec CSS
	resumeBtn->get_style_context()->add_class("button");
	exitBtn->get_style_context()->add_class("button");
	restartBtn->get_style_context()->add_class("button");

	// Paramètrage de la boite principale
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(true);
	wrapper->set_vexpand(true);

	// Paramètrage de la sous boite
	scoreBox->set_homogeneous(true);
	scoreBox->set_hexpand(false);

	// Ajouter les widget au wrapper
	scoreBox->pack_start(*scoreLabelOverMenu, Gtk::PACK_SHRINK, 0);
	scoreBox->pack_start(*bestScoreLabelPauseMenu, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*scoreBox, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*resumeBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*restartBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*exitBtn, Gtk::PACK_SHRINK, 0);
	wrapper->override_background_color(Gdk::RGBA(GAMEOVER_BKG_COLOR), Gtk::STATE_FLAG_NORMAL);

	return wrapper;
}

void GameWindow::ExitGame()
{
	this->close();
}

void GameWindow::RestartGame()
{

	// On cache les widget déjà présent
	HideAllGtkWidgets();
	// On affiche le terrain
	gameBoard->show();

	// On reconnecte
	ReconnectGameControls();

	// On réinitialise le gestionnaire de temps
	timeManager.Reset();

	// On réinitialise le terrain pour une nouvelle partie
	terrainPiece.terrainGraph->ResetTerrain(terrainPiece.pieceGraph, timeManager);

	// On reinitialise les préview
	terrainPiece.previewGraph->RenderGrid(timeManager.SeeNextPiece());
	terrainPiece.previousPreviewGraph->RenderGrid(NULL);

	// On réinitialise le timeout de la mainGameLoop
	currentMainLoopTimeout = MAIN_LOOP_TIMEOUT;

	// On actualise l'affichage du score
	RenderScore(1);
}

void GameWindow::MainMenuButton()
{
	HideAllGtkWidgets();

	// DisconnectGameControls();
	mainMenu->show();
}

void GameWindow::StartButton()
{
	// On ajoute le terrain à l'overlay s'il n'y est pas déjà
	if (gameBoard->get_parent() == 0)
	{
		overlay->add(*gameBoard);
		overlay->show_all();
	}

	RestartGame();
}

void GameWindow::ResumeButton()
{
	HideAllGtkWidgets();
	gameBoard->show();
	ReconnectGameControls();
}

void GameWindow::PauseButton()
{
	if (isInGame)
	{
		if (pauseMenu == NULL)
		{
			// Ajout du menu Pause à l'overlay
			pauseMenu = MakePauseMenu();
			overlay->add_overlay(*pauseMenu);

			// Quand on ajoute un widget à l'overlay, GTK ajoute un composant intermédiaire enfant de l'overlay et dont le widget est enfant
			// Ainsi, il est nécessaire d'afficher cet enfant intermédiare dont on n'a aucune information
			// C'est pour cela que l'on utilise show_all(), puis que l'on cache les widgets inutiles
			overlay->show_all();
		}

		DisconnectGameControls();

		// On affiche le menu pause au dessus du menu de jeu
		HideAllGtkWidgets();
		pauseMenu->show();
		gameBoard->show();
	}
}

void GameWindow::HideAllGtkWidgets() const
{
	if (gameOverMenu != NULL)
		gameOverMenu->hide();

	if (mainMenu != NULL)
		mainMenu->hide();

	if (gameBoard != NULL)
		gameBoard->hide();

	if (pauseMenu != NULL)
		pauseMenu->hide();
}

void GameWindow::DisconnectGameControls()
{
	// On est plus dans le jeu
	isInGame = false;

	// On bloque les entrées clavier
	keyboardControls.block();

	// On déconnecte la boucle de jeu
	// Un block sur une connection de type signal_timeout fait perdre la connection et est donc impossible à reprendre
	// On utilise donc directement disconnect
	mainGameLoop.disconnect();
}

void GameWindow::ReconnectGameControls()
{
	// On est a nouveau dans le jeu
	isInGame = true;

	// On recrée la connection lancant la boucle principale
	mainGameLoop = Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::MainGameLoop), MAIN_LOOP_TIMEOUT);

	// On débloque les entrées clavier
	keyboardControls.unblock();
}

void GameWindow::ActualiseTimeStickers()
{
	// On modifie les vignettes pour correspondre à la variable de temps
	int time = timeManager.GetTimePosition();

	for (int i = 0; i <= time; i++)
	{
		// Les premières vignettes sont pleines
		timeLabels[i].get_style_context()->remove_class("timeStickersempty");
		timeLabels[i].get_style_context()->add_class("timeStickers");
	}

	for (int i = time + 1; i < MAX_PREDICTION; i++)
	{
		// Les autres sont vides
		timeLabels[i].get_style_context()->remove_class("timeStickersempty");
		timeLabels[i].get_style_context()->add_class("timeStickersempty");
	}
}

void GameWindow::RenderScore(const bool isGameBoard) const
{
	// Réupération des données
	std::string scorestr = std::to_string(terrainPiece.terrainGraph->GetScore());

	if (isGameBoard)
	{
		// Réupération des données
		std::string levelstr = std::to_string(1 + terrainPiece.terrainGraph->GetClearedLines() / 10);

		// Assignation des textes aux label
		scoreLabel->set_text("Score: " + scorestr);
		levelLabel->set_text("Level: " + levelstr);
	}
	else
	{
		// Réupération des données
		std::string bscorestr = std::to_string(bestScore);

		// Assignation des textes aux label
		scoreLabelOverMenu->set_text("Score: " + scorestr);
		bestScoreLabelOverMenu->set_text("Best Score: " + bscorestr);
		bestScoreLabelMainMenu->set_text("Best Score: " + bscorestr);
		bestScoreLabelPauseMenu->set_text("Best Score: " + bscorestr);
	}
}

bool TryMovePieceDown(const TerrainPiece &data)
{
	Piece *piece = (Piece *)(*data.pieceGraph);
	Terrain *terrain = (Terrain *)(data.terrainGraph);

	// Check si la pièce peut descendre
	piece->Move(0, 1);
	if (terrain->CheckCollision(piece))
	{
		piece->Move(0, -1);
		return false;
	}

	return true;
}

// Fonction appelée toute les MAIN_LOOP_TIMEOUT ms tant qu'elle retourne true
bool GameWindow::MainGameLoop()
{
	// Si la piece n'a pas pu etre descendu, alors il y a un obstacle l'empechant -> La piece doit etre placée
	if (!TryMovePieceDown(terrainPiece))
	{
		// S'il y a eu une collision quand on a placé la pièce
		if (!terrainPiece.terrainGraph->ImprintPiece(terrainPiece.pieceGraph, timeManager))
		{
			GameOver();

			// On arrête le signal timeout
			return false;
		}

		// On actualise la preview de la pièce future
		terrainPiece.previewGraph->RenderGrid(timeManager.SeeNextPiece());

		Piece *piece = (Piece *)(*terrainPiece.pieceGraph);
		Terrain *terrain = (Terrain *)(terrainPiece.terrainGraph);

		// Si la piece nouvellement spawn touche un bloc -> Game over
		if (terrain->CheckCollision(piece))
		{
			GameOver();

			// On arrête le signal timeout
			return false;
		}
	}

	// On actualise l'affichage du score et du terrain
	RenderScore(1);
	terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

	// On augmente la vitesse de jeu selon le nombre de lignes complétées
	int clearedLines = terrainPiece.terrainGraph->GetClearedLines();

	if (clearedLines % 10 == 0)
	{
		currentMainLoopTimeout = std::max(MAIN_LOOP_TIMEOUT / 20, MAIN_LOOP_TIMEOUT - 50 * (clearedLines / 10));

		mainGameLoop.disconnect();
		mainGameLoop = Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::MainGameLoop), currentMainLoopTimeout);

		// On arrète le signal timeout
		return false;
	}

	return true;
}

// Réinitialise le temps à la prochaine itération de MainGameLoop
void GameWindow::ResetMainGameLoop()
{
	mainGameLoop.disconnect();
	mainGameLoop = Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::MainGameLoop), currentMainLoopTimeout);
}

void GameWindow::GameOver()
{
	// Actualisation du bestScore
	if (bestScore < terrainPiece.terrainGraph->GetScore())
		bestScore = terrainPiece.terrainGraph->GetScore();

	// Actualisation des label affichant le best score
	RenderScore(0);

	// Création du menu gameOver
	if (gameOverMenu == NULL)
	{
		// Ajout du menu Game Over à l'overlay
		gameOverMenu = MakeGameOverMenu();
		overlay->add_overlay(*gameOverMenu);

		// Quand on ajoute un widget à l'overlay, GTK ajoute un composant intermédiaire enfant de l'overlay et dont le widget est enfant
		// Ainsi, il est nécessaire d'afficher cet enfant intermédiare dont on n'a aucune information
		// C'est pour cela que l'on utilise show_all(), puis que l'on cache les widgets inutiles
		overlay->show_all();
	}

	DisconnectGameControls();

	// On affiche le menu game over au dessus du menu de jeu
	HideAllGtkWidgets();
	gameOverMenu->show();
	gameBoard->show();
}

bool GameWindow::OnKeyPress(GdkEventKey *const event)
{
	bool signalHandled = false;

	Piece *piece = (Piece *)(*terrainPiece.pieceGraph);
	Terrain *terrain = (Terrain *)(terrainPiece.terrainGraph);

	switch (event->keyval)
	{
		// Fais tourner la pièce vers la gauche
		case GDK_KEY_a:
			piece->RotateLeft();
			// Implémentation du 'Wall kick' si on essaye de tourner alors qu'il y a un obstacle
			if (terrain->CheckCollision(piece))
			{
				piece->Move(1, 0);
				if (terrain->CheckCollision(piece))
				{
					piece->Move(-2, 0);
					if (terrain->CheckCollision(piece))
					{
						piece->Move(1, 0);
						piece->RotateRight();

						return true;
					}
				}
			}

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

			signalHandled = true;
			break;

		// Implémentation du "hard drop" la pièce tombe instantanément
		case GDK_KEY_x:

			while (!terrain->CheckCollision(piece))
				piece->Move(0, 1);

			piece->Move(0, -1);

			ResetMainGameLoop();
			MainGameLoop();

			signalHandled = true;
			break;

		// Fais tourner la pièce vers la gauche
		case GDK_KEY_z:
			piece->RotateRight();
			// Implémentation du 'Wall kick' si on essaye de tourner alors qu'il y a un obstacle
			if (terrain->CheckCollision(piece))
			{
				piece->Move(1, 0);
				if (terrain->CheckCollision(piece))
				{
					piece->Move(-2, 0);
					if (terrain->CheckCollision(piece))
					{
						piece->Move(1, 0);
						piece->RotateLeft();

						signalHandled = true;
					}
				}
			}

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

			signalHandled = true;
			break;

		// Déplace la pièce a gauche
		case GDK_KEY_Left:
			piece->Move(-1, 0);
			if (terrain->CheckCollision(piece))
				piece->Move(1, 0);

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

			signalHandled = true;
			break;

		// Déplace la pièce a droite
		case GDK_KEY_Right:
			piece->Move(1, 0);
			if (terrain->CheckCollision(piece))
				piece->Move(-1, 0);

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

			signalHandled = true;
			break;

		// Déplace la pièce vers le bas
		case GDK_KEY_Down:
			// Actualise le terrain seulement si la piece a été déplacée
			if (TryMovePieceDown(terrainPiece))
				terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			else
			{
				ResetMainGameLoop();
				MainGameLoop();
			}

			signalHandled = true;
			break;

		// Lance le menu pause
		case GDK_KEY_p:
			PauseButton();
			signalHandled = true;
			break;

		//	Avance dans le temps
		case GDK_KEY_s:
			// S'il y a eu une collsion lors du changement de temporalité
			if (!timeManager.MoveInTime(terrainPiece.pieceGraph, terrainPiece.terrainGraph))
				GameOver();

			// Si la piece nouvellement spawn touche un bloc -> Game over
			if (terrain->CheckCollision((Piece *)(*terrainPiece.pieceGraph)))
				GameOver();

			// On affiche la pièce et actualise les preview
			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			terrainPiece.previewGraph->RenderGrid(timeManager.SeeNextPiece());
			terrainPiece.previousPreviewGraph->RenderGrid(timeManager.SeePreviousPiece());

			ActualiseTimeStickers();
			signalHandled = true;
			break;

		// Reculer dans le temps
		case GDK_KEY_q:
			timeManager.BackInTime(terrainPiece.pieceGraph, terrainPiece.terrainGraph);

			// Si la piece nouvellement spawn touche un bloc -> Game over
			if (terrain->CheckCollision((Piece *)(*terrainPiece.pieceGraph)))
				GameOver();

			// On affiche la pièce et actualise les preview
			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			terrainPiece.previewGraph->RenderGrid(timeManager.SeeNextPiece());
			terrainPiece.previousPreviewGraph->RenderGrid(timeManager.SeePreviousPiece());

			ActualiseTimeStickers();
			signalHandled = true;
			break;
	}

	// Si l'évènement n'a pas été géré, appeler la classe de base
	if (!signalHandled)
		return Gtk::Window::on_key_press_event(event);
	else
		return true;
}
