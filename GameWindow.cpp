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

GameWindow::GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing, const TerrainPiece *tp) : terrainPiece(tp)
{
	// Paramétrage de la fenetre
	set_title(name);
	set_default_size(width, height);
	set_resizable(true);
	set_border_width(borderSize);
	set_position(Gtk::WIN_POS_CENTER);
	override_background_color(Gdk::RGBA(BACKGROUND_COLOR), Gtk::StateFlags::STATE_FLAG_NORMAL);

	// Creation de la grille de jeu
	terrainGrid = Gtk::make_managed<Gtk::Grid>(); // Laisse Gtk delete le terrain quand la fenetre se ferme
	SetupGrid(terrainGrid, gridSpacing);

	// Création de l'overlay
	overlay = Gtk::make_managed<Gtk::Overlay>();
	overlay->add(*terrainGrid);
	add(*overlay); // Ajout de l'overlay à la fenêtre

	// Gestion des entrées clavier
	add_events(Gdk::KEY_PRESS_MASK);
	keyboardControls = signal_key_press_event().connect(sigc::bind<-1>(sigc::mem_fun(*this, &GameWindow::OnKeyPress), tp), false);
	// m_entry.signal_key_press_event().connect(sigc::mem_fun(this, &GameWindow::OnKeyPress), tp);

	// Boucle de jeu principale
	mainGameLoop = Glib::signal_timeout().connect(sigc::bind<-1>(sigc::mem_fun(*this, &GameWindow::MainGameLoop), tp), MAIN_LOOP_TIMEOUT);
}

GameWindow::~GameWindow()
{
	keyboardControls.disconnect();
	mainGameLoop.disconnect();
}

void GameWindow::RetryButton()
{
	// On enlève le menu Game Over
	gameOverMenu->hide();

	// On recrée la connection lancant la boucle principale
	mainGameLoop = Glib::signal_timeout().connect(sigc::bind<-1>(sigc::mem_fun(*this, &GameWindow::MainGameLoop), terrainPiece), MAIN_LOOP_TIMEOUT);

	// On débloque les entrées clavier
	keyboardControls.unblock();

	// On réinitialise le terrain pour une nouvelle partie
	terrainPiece->terrainGraph->ResetTerrain(terrainPiece->pieceGraph);
}

Gtk::Box *GameWindow::MakeGameOverMenu()
{
	Gtk::Box *wrapper = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);
	Gtk::Label *gameOverLabel = Gtk::make_managed<Gtk::Label>();
	Gtk::Button *retryBtn = Gtk::make_managed<Gtk::Button>("Retry");
	Gtk::Button *mainMenuBtn = Gtk::make_managed<Gtk::Button>("Main menu");
	Gtk::Button *exitBtn = Gtk::make_managed<Gtk::Button>("Exit");

	// Utiliser CSS pour éviter de hardcoder la taille
	gameOverLabel->set_text("<span size='34000'>Game Over !</span>");
	gameOverLabel->set_use_markup(true);
	gameOverLabel->override_color(Gdk::RGBA(GAMEOVER_COLOR), Gtk::STATE_FLAG_NORMAL);

	// gameOverLabel->set_hexpand(true);
	// gameOverLabel->set_halign(Gtk::Align::ALIGN_CENTER);
	// gameOverLabel->set_vexpand(true);
	// gameOverLabel->set_valign(Gtk::Align::ALIGN_CENTER);

	// Style : A faire avec CSS
	wrapper->set_homogeneous(true);
	wrapper->set_hexpand(false);
	wrapper->set_margin_bottom(100);

	// Connecte les boutons aux actions à réaliser
	retryBtn->signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::RetryButton));

	wrapper->pack_start(*gameOverLabel, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*retryBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*mainMenuBtn, Gtk::PACK_SHRINK, 0);
	wrapper->pack_start(*exitBtn, Gtk::PACK_SHRINK, 0);

	return wrapper;
}

void GameWindow::GameOver()
{
	if (gameOverMenu == NULL)
	{
		gameOverMenu = MakeGameOverMenu();
		overlay->add_overlay(*gameOverMenu);
		overlay->show_all();
	}
	else
		gameOverMenu->show();

	keyboardControls.block();
	// Un block sur une connection de type signal_timeout revient à un disconnect: on perd la connection
	mainGameLoop.disconnect();
}

bool TryMovePieceDown(const TerrainPiece *data)
{
	Piece *piece = (Piece *)(*data->pieceGraph);
	Terrain *terrain = (Terrain *)(data->terrainGraph);

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
bool GameWindow::MainGameLoop(const TerrainPiece *data)
{
	// Si la piece n'a pas pu etre descendu, alors il y a un obstacle l'empechant -> La piece doit etre placée
	if (!TryMovePieceDown(data))
	{
		data->terrainGraph->ImprintPiece(data->pieceGraph);

		Piece *piece = (Piece *)(*data->pieceGraph);
		Terrain *terrain = (Terrain *)(data->terrainGraph);

		// Si la piece nouvellement spawn touche un bloc -> Game over
		if (terrain->CheckCollision(piece))
			GameOver();
	}

	data->terrainGraph->RenderTerrain(*data->pieceGraph);

	return true;
}

bool GameWindow::OnKeyPress(GdkEventKey *const event, const TerrainPiece *data)
{
	Piece *piece = (Piece *)(*data->pieceGraph);
	Terrain *terrain = (Terrain *)(data->terrainGraph);

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

		data->terrainGraph->RenderTerrain(*data->pieceGraph);
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

		data->terrainGraph->RenderTerrain(*data->pieceGraph);
		return true;
		break;
	case GDK_KEY_Left:
		piece->Move(-1, 0);
		if (terrain->CheckCollision(piece))
			piece->Move(1, 0);

		data->terrainGraph->RenderTerrain(*data->pieceGraph);
		return true;
		break;
	case GDK_KEY_Right:
		piece->Move(1, 0);
		if (terrain->CheckCollision(piece))
			piece->Move(-1, 0);

		data->terrainGraph->RenderTerrain(*data->pieceGraph);
		return true;
		break;
	case GDK_KEY_Down:
		// Actualise le terrain seulement si la piece a été déplacée
		if (TryMovePieceDown(data))
			data->terrainGraph->RenderTerrain(*data->pieceGraph);

		return true;
		break;
	}
	// Si l'évènement n'a pas été gérer, appeler la classe de base
	return Gtk::Window::on_key_press_event(event);
}

Gtk::Grid *GameWindow::GetGrid() const
{
	return terrainGrid;
}