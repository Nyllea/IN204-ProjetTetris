#include "GameWindow.hpp"
#include <gtkmm-3.0/gtkmm/cssprovider.h>

void SetupGrid(Gtk::Grid *grid, const guint spacing)
{
	grid->set_row_homogeneous(false);
	grid->set_column_homogeneous(false);

	grid->set_column_spacing(spacing);
	grid->set_row_spacing(spacing);

	// grid->set_name(name);

	grid->set_hexpand(true);
	grid->set_halign(Gtk::Align::ALIGN_CENTER);
	grid->set_vexpand(true);
	grid->set_valign(Gtk::Align::ALIGN_CENTER);
}

GameWindow::GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing) : gameOverMenu(NULL), bestScore(0)
{
	// Paramétrage de la fenetre
	set_title(name);
	set_default_size(width, height);
	set_resizable(false);
	set_border_width(borderSize);
	set_position(Gtk::WIN_POS_CENTER);
	override_background_color(Gdk::RGBA(BACKGROUND_COLOR), Gtk::StateFlags::STATE_FLAG_NORMAL);


	// Initialisation du provider
	// datas= ".startBtn {background-image: url('GObackground.png'); color: red; border:solid 2px #ffffff; border-radius: 7px; border: 7px; padding: 15px 32px; }";
	provider=Gtk::CssProvider::create();
	provider->load_from_path("Data.css");

	// Génération des classes de jeu
	terrainPiece.terrainGraph = new TerrainGraphic();
	terrainPiece.previewGraph = new PreviewGraphic();
	terrainPiece.previousPreviewGraph = new PreviewGraphic();

	// Génération des pièces futures et assignation d'une zone mémoire pour la piece actuelle
	piecesManager.GeneratePieces();
	terrainPiece.pieceGraph = new (PieceGraphic *)(piecesManager.GetNextPiece());

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
	bestScoreLabel = Gtk::make_managed<Gtk::Label>();
	levelLabel = Gtk::make_managed<Gtk::Label>();
	scoreLabelOverMenu = Gtk::make_managed<Gtk::Label>();
	bestScoreLabelOverMenu = Gtk::make_managed<Gtk::Label>();
	bestScoreLabelMainMenu = Gtk::make_managed<Gtk::Label>();

	// initialisation de l'affichage des scores
	RenderScore(0);
	RenderScore(1);

	levelLabel->set_text("<span size='34000'>level : </span>");
	levelLabel->set_use_markup(true);
	levelLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

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

void GameWindow::ExitGame()
{
	this->close();
}

void GameWindow::RestartGame()
{
	// On cache les widget déjà présent
	HideAll();
	// On affiche le terrain
	gameBoard->show();

	ReconnectGameControls();

	// On réinitialise le terrain pour une nouvelle partie
	terrainPiece.terrainGraph->ResetTerrain(terrainPiece.pieceGraph, piecesManager);
	terrainPiece.previewGraph->RenderGrid(piecesManager.SeeNextPiece());
}

void GameWindow::MainMenuButton()
{
	HideAll();

	DisconnectGameControls();
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

Gtk::Box *GameWindow::MakeGameBoard(Gtk::Grid *terrainGrid, Gtk::Grid *previewGrid, Gtk::Grid *previousPreviewGrid)
{
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

	Gtk::Box *leftWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Box *rightWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

	// Gtk::Label *scoreLabel = Gtk::make_managed<Gtk::Label>();
	// Gtk::Label *bestScoreLabel = Gtk::make_managed<Gtk::Label>();

	// Utiliser CSS pour éviter de hardcoder la taille
	// scoreLabel->set_text("<span size='34000'>Score : </span>");
	// scoreLabel->set_use_markup(true);
	// scoreLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	// bestScoreLabel->set_text("<span size='34000'>Best Score : </span>");
	// bestScoreLabel->set_use_markup(true);
	// bestScoreLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	leftWrapper->set_homogeneous(true);
	leftWrapper->set_hexpand(false);
	leftWrapper->pack_start(*scoreLabel, Gtk::PACK_SHRINK, 0);
	// leftWrapper->pack_start(*scoreValue);
	leftWrapper->pack_start(*previousPreviewGrid, Gtk::PACK_SHRINK, 0);

	rightWrapper->set_homogeneous(true);
	rightWrapper->set_hexpand(false);
	rightWrapper->pack_start(*bestScoreLabel, Gtk::PACK_SHRINK, 0);
	rightWrapper->pack_start(*levelLabel, Gtk::PACK_SHRINK, 0);
	rightWrapper->pack_start(*previewGrid, Gtk::PACK_SHRINK, 0);

	// Style : A faire avec CSS
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	// wrapper->set_margin_bottom(100);

	// Ajouter les widget au wrapper
	wrapper->pack_start(*leftWrapper, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*terrainGrid, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*rightWrapper, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

Gtk::Box *GameWindow::MakeMainMenu()
{
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *mainMenuLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Label *bestScoreLabel2 = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *startBtn = Gtk::make_managed<Gtk::Button>("Start Game");
	// Gtk::Button *optionBtn = Gtk::make_managed<Gtk::Button>("Options");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Utiliser CSS pour éviter de hardcoder la taille
	mainMenuLabel->set_text("<span size='34000'>Main Menu</span>");
	mainMenuLabel->set_use_markup(true);
	mainMenuLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	std::string scorestr = std::to_string(bestScore);
	bestScoreLabel2->set_text("<span size='34000'>Best Score : " + scorestr + "</span>");
	bestScoreLabel2->set_use_markup(true);
	bestScoreLabel2->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	//CSS style
	auto wrapperStyleContext = wrapper->get_style_context();
	styleContext->add_class("mainMenuBackgroud");
	styleContext->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

	// Style : A faire avec CSS
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	wrapper->set_margin_bottom(100);

	// Connecte les boutons aux actions à réaliser
	startBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::StartButton));
	// optionBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::OptionButton));
	exitBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ExitGame));

	// Ajouter les widget au wrapper
	wrapper->pack_start(*mainMenuLabel, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*bestScoreLabelMainMenu, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*startBtn, Gtk::PACK_SHRINK, 0);

	// wrapper->pack_start(*optionBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*exitBtn, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

Gtk::Box *GameWindow::MakeGameOverMenu()
{

	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *gameOverLabel = Gtk::make_managed<Gtk::Label>();

	Gtk::Box *scoreBox = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
	Gtk::Label *locScoreLabel = Gtk::make_managed<Gtk::Label>();
	// Gtk::Label *bestScoreLabel = Gtk::make_managed<Gtk::Label>();

	Gtk::Button *retryBtn = Gtk::make_managed<Gtk::Button>("Retry");
	Gtk::Button *mainMenuBtn = Gtk::make_managed<Gtk::Button>("Main menu");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Utiliser CSS pour éviter de hardcoder la taille
	gameOverLabel->set_text("<span size='34000'>Game Over !</span>");
	gameOverLabel->set_use_markup(true);
	gameOverLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	// std::string scorestr=std::to_string(score); //converting number to a string
	// locScoreLabel->set_text("<span size='34000'>Score :"+scorestr+" 0</span>");
	// locScoreLabel->set_use_markup(true);
	// locScoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	// bestScoreLabel->set_text("<span size='34000'>Best Score : 0</span>");
	// bestScoreLabel->set_use_markup(true);
	// bestScoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	// gameOverLabel->set_hexpand(true);
	// gameOverLabel->set_halign(Gtk::Align::ALIGN_CENTER);
	// gameOverLabel->set_vexpand(true);
	// gameOverLabel->set_valign(Gtk::Align::ALIGN_CENTER);

	// Style : A faire avec CSS
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	wrapper->set_margin_bottom(100);

	scoreBox->set_homogeneous(true);
	scoreBox->set_hexpand(false);

	// Connecte les boutons aux actions à réaliser
	retryBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::RestartGame));
	mainMenuBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::MainMenuButton));
	exitBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ExitGame));

	auto blublu = wrapper->get_style_context();
	blublu->add_class("startBtn");
	blublu->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

	// Ajouter les widget au wrapper
	wrapper->pack_start(*gameOverLabel, Gtk::PACK_SHRINK, 0);

	scoreBox->pack_start(*scoreLabelOverMenu, Gtk::PACK_SHRINK, 0);
	scoreBox->pack_start(*bestScoreLabelOverMenu, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*scoreBox, Gtk::PACK_SHRINK, 0);

	wrapper->pack_start(*retryBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*mainMenuBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*exitBtn, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

void GameWindow::HideAll()
{
	if (gameOverMenu != NULL)
		gameOverMenu->hide();

	if (mainMenu != NULL)
		mainMenu->hide();

	if (gameBoard != NULL)
		gameBoard->hide();
}

void GameWindow::DisconnectGameControls()
{
	// On bloque les entrées clavier
	keyboardControls.block();

	// On déconnecte la boucle de jeu
	// Un block sur une connection de type signal_timeout revient à un disconnect: on perd la connection
	mainGameLoop.disconnect();
}

void GameWindow::UpdateSpeed(int cplt_lines)
{
	int TIME = 1000 / (terrainPiece.terrainGraph->GetClearedLines() + 1);

	if (cplt_lines % 10 == 0)
	{
		mainGameLoop.disconnect();
		mainGameLoop = Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::MainGameLoop), TIME);
	}
}

void GameWindow::ReconnectGameControls()
{
	// On recrée la connection lancant la boucle principale
	mainGameLoop = Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::MainGameLoop), MAIN_LOOP_TIMEOUT);

	// On débloque les entrées clavier
	keyboardControls.unblock();
}

void GameWindow::GameOver()
{
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

	HideAll();

	DisconnectGameControls();

	// On affiche le menu game over
	gameOverMenu->show();
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
		terrainPiece.terrainGraph->ImprintPiece(terrainPiece.pieceGraph, piecesManager);
		terrainPiece.previewGraph->RenderGrid(piecesManager.SeeNextPiece());

		Piece *piece = (Piece *)(*terrainPiece.pieceGraph);
		Terrain *terrain = (Terrain *)(terrainPiece.terrainGraph);

		// Si la piece nouvellement spawn touche un bloc -> Game over
		if (terrain->CheckCollision(piece))
		{
			if (bestScore < terrainPiece.terrainGraph->GetScore())
			{
				bestScore = terrainPiece.terrainGraph->GetScore();
			}
			RenderScore(1);
			// gameBoard->remove(*bestScoreLabel);
			// bestScoreLabel->set_parent(*gameOverMenu);
			GameOver();

			// On arrête le signal timeout
			return false;
		}
	}

	RenderScore(0);
	UpdateSpeed(terrainPiece.terrainGraph->GetClearedLines());

	terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

	return true;
}

void GameWindow::RenderScore(int spec)
{
	// int score=terrainPiece.terrainGraph->GetScore();
	// int lines=terrainPiece.terrainGraph->GetClearedLines();
	// int bscore=bestScore;
	std::string scorestr = std::to_string(terrainPiece.terrainGraph->GetScore());
	std::string levelstr = std::to_string(1 + terrainPiece.terrainGraph->GetClearedLines() / 10);
	std::string bscorestr = std::to_string(bestScore);

	switch (spec)
	{
		case 0:
			scoreLabel->set_text("<span size='34000'>Score: " + scorestr + " </span>");
			scoreLabel->set_use_markup(true);
			scoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			levelLabel->set_text("<span size='34000'>Level: " + levelstr + " </span>");
			levelLabel->set_use_markup(true);
			scoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			break;

		case 1:
			scoreLabelOverMenu->set_text("<span size='34000'>Score: " + scorestr + " </span>");
			scoreLabelOverMenu->set_use_markup(true);
			scoreLabelOverMenu->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			bestScoreLabelOverMenu->set_text("<span size='34000'>Best Score: " + bscorestr + " </span>");
			bestScoreLabelOverMenu->set_use_markup(true);
			bestScoreLabelOverMenu->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			bestScoreLabelMainMenu->set_text("<span size='34000'>Best Score: " + bscorestr + " </span>");
			bestScoreLabelMainMenu->set_use_markup(true);
			bestScoreLabelMainMenu->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			bestScoreLabel->set_text("<span size='34000'>Best Score: " + bscorestr + " </span>");
			bestScoreLabel->set_use_markup(true);
			bestScoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

			break;
	}
}

bool GameWindow::OnKeyPress(GdkEventKey *const event)
{
	Piece *piece = (Piece *)(*terrainPiece.pieceGraph);
	Terrain *terrain = (Terrain *)(terrainPiece.terrainGraph);

	switch (event->keyval)
	{
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
			return true;
			break;

		case GDK_KEY_x:
			// Implémentation du "hard drop" la pièce tombe instantanément

			while (!terrain->CheckCollision(piece))
			{
				piece->Move(0, 1);
			}
			piece->Move(0, -1);
			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			return true;
			break;

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

						return true;
					}
				}
			}

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			return true;
			break;
		case GDK_KEY_Left:
			piece->Move(-1, 0);
			if (terrain->CheckCollision(piece))
				piece->Move(1, 0);

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			return true;
			break;
		case GDK_KEY_Right:
			piece->Move(1, 0);
			if (terrain->CheckCollision(piece))
				piece->Move(-1, 0);

			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			return true;
			break;
		case GDK_KEY_Down:
			// Actualise le terrain seulement si la piece a été déplacée
			if (TryMovePieceDown(terrainPiece))
				terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

			return true;
			break;
		case GDK_KEY_e:
			piecesManager.MoveInTime(terrainPiece.pieceGraph);

			// Si la piece nouvellement spawn touche un bloc -> Game over
			if (terrain->CheckCollision((Piece *)(*terrainPiece.pieceGraph)))
				GameOver();

			// On affiche la pièce et actualise les preview
			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			terrainPiece.previewGraph->RenderGrid(piecesManager.SeeNextPiece());
			terrainPiece.previousPreviewGraph->RenderGrid(piecesManager.SeePreviousPiece());

			return true;
			break;
		case GDK_KEY_d:
			piecesManager.BackInTime(terrainPiece.pieceGraph);

			// Si la piece nouvellement spawn touche un bloc -> Game over
			if (terrain->CheckCollision((Piece *)(*terrainPiece.pieceGraph)))
				GameOver();

			// On affiche la pièce et actualise les preview
			terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);
			terrainPiece.previewGraph->RenderGrid(piecesManager.SeeNextPiece());
			terrainPiece.previousPreviewGraph->RenderGrid(piecesManager.SeePreviousPiece());

			return true;
			break;
	}
	// Si l'évènement n'a pas été gérer, appeler la classe de base
	return Gtk::Window::on_key_press_event(event);
}