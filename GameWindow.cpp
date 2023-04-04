#include "GameWindow.hpp"

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

GameWindow::GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing) : gameOverMenu(NULL)
{
	// Paramétrage de la fenetre
	set_title(name);
	set_default_size(width, height);
	set_resizable(true);
	set_border_width(borderSize);
	set_position(Gtk::WIN_POS_CENTER);
	override_background_color(Gdk::RGBA(BACKGROUND_COLOR), Gtk::StateFlags::STATE_FLAG_NORMAL);

	// Génération des classes de jeu
	terrainPiece.terrainGraph = new TerrainGraphic();
	terrainPiece.previewGraph = new PreviewGraphic();

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

	// Création du terrain de jeu
	gameBoard = MakeGameBoard(m_terrainGrid, m_previewGrid);

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

Gtk::Box *GameWindow::MakeGameBoard(Gtk::Grid *terrainGrid, Gtk::Grid *previewGrid)
{
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

	Gtk::Box *scoreWrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *scoreLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Label *bestScoreLabel = Gtk::make_managed<Gtk::Label>();

	// Utiliser CSS pour éviter de hardcoder la taille
	scoreLabel->set_text("<span size='34000'>Score : </span>");
	scoreLabel->set_use_markup(true);
	scoreLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	bestScoreLabel->set_text("<span size='34000'>Best Score : </span>");
	bestScoreLabel->set_use_markup(true);
	bestScoreLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	scoreWrapper->set_homogeneous(true);
	scoreWrapper->set_hexpand(false);

	scoreWrapper->pack_start(*scoreLabel, Gtk::PACK_SHRINK, 0);
	scoreWrapper->pack_start(*bestScoreLabel, Gtk::PACK_SHRINK, 0);

	// Style : A faire avec CSS
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	// wrapper->set_margin_bottom(100);

	// Ajouter les widget au wrapper
	wrapper->pack_start(*scoreWrapper, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*terrainGrid, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*previewGrid, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

Gtk::Box *GameWindow::MakeMainMenu()
{
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *mainMenuLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Label *bestScoreLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *startBtn = Gtk::make_managed<Gtk::Button>("Start Game");
	// Gtk::Button *optionBtn = Gtk::make_managed<Gtk::Button>("Options");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Utiliser CSS pour éviter de hardcoder la taille
	mainMenuLabel->set_text("<span size='34000'>Main Menu</span>");
	mainMenuLabel->set_use_markup(true);
	mainMenuLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

	bestScoreLabel->set_text("<span size='34000'>Best Score : </span>");
	bestScoreLabel->set_use_markup(true);
	bestScoreLabel->override_color(Gdk::RGBA(MAINMENU_COLOR), Gtk::STATE_FLAG_NORMAL);

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
	Gtk::Label *scoreLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Label *bestScoreLabel = Gtk::make_managed<Gtk::Label>();

	Gtk::Button *retryBtn = Gtk::make_managed<Gtk::Button>("Retry");
	Gtk::Button *mainMenuBtn = Gtk::make_managed<Gtk::Button>("Main menu");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Utiliser CSS pour éviter de hardcoder la taille
	gameOverLabel->set_text("<span size='34000'>Game Over !</span>");
	gameOverLabel->set_use_markup(true);
	gameOverLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	scoreLabel->set_text("<span size='34000'>Score : </span>");
	scoreLabel->set_use_markup(true);
	scoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	bestScoreLabel->set_text("<span size='34000'>Best Score : </span>");
	bestScoreLabel->set_use_markup(true);
	bestScoreLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

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

	// Ajouter les widget au wrapper
	wrapper->pack_start(*gameOverLabel, Gtk::PACK_SHRINK, 0);

	scoreBox->pack_start(*scoreLabel, Gtk::PACK_SHRINK, 0);
	scoreBox->pack_start(*bestScoreLabel, Gtk::PACK_SHRINK, 0);
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
			GameOver();
	}

	terrainPiece.terrainGraph->RenderGrid(*terrainPiece.pieceGraph);

	return true;
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
	}
	// Si l'évènement n'a pas été gérer, appeler la classe de base
	return Gtk::Window::on_key_press_event(event);
}