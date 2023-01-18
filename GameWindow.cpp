#include "GameWindow.hpp"

void SetupGrid(Gtk::Grid *grid, const guint spacing){
    grid->set_row_homogeneous(false);
    grid->set_column_homogeneous(false);

    grid->set_column_spacing(spacing);
    grid->set_row_spacing(spacing);

    //grid->set_name(name);

    grid->set_hexpand(true);
    grid->set_halign(Gtk::Align::ALIGN_CENTER);
    grid->set_vexpand(true);
    grid->set_valign(Gtk::Align::ALIGN_CENTER);
}

GameWindow::GameWindow (const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing, const TerrainPiece* tp) {
    // Paramétrage de la fenetre
    set_title(name);
    set_default_size(width, height);
    set_resizable(true);
    set_border_width(borderSize);
    set_position(Gtk::WIN_POS_CENTER);

    // Creation de la grille de jeu
    terrainGrid = Gtk::make_managed<Gtk::Grid>(); // Laisse Gtk delete le terrain quand la fenetre se ferme
    SetupGrid(terrainGrid, gridSpacing);
    add(*terrainGrid); //Ajout à la fenêtre

    // Gestion des entrées clavier
	add_events(Gdk::KEY_PRESS_MASK);
	signal_key_press_event().connect(sigc::bind<-1>(sigc::mem_fun(*this, &GameWindow::OnKeyPress), tp), false);
    //m_entry.signal_key_press_event().connect(sigc::mem_fun(this, &GameWindow::OnKeyPress), tp);
}

bool GameWindow::OnKeyPress (GdkEventKey* const event, const TerrainPiece* data) {
    switch(event->keyval){
		case GDK_KEY_space:
			data->terrain->ImprintPiece(data->piece);
        	return TRUE;
			break;
		case GDK_KEY_a:
			((Piece*)(*data->piece))->RotateLeft();
			// Implémentation du 'Wall kick' si on essaye de tourner alors qu'il y a un obstacle
			if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
				((Piece*)(*data->piece))->Move(1, 0);
				if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
					((Piece*)(*data->piece))->Move(-2, 0);
					if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
						((Piece*)(*data->piece))->Move(1, 0);
						((Piece*)(*data->piece))->RotateRight();
					}
				}
			}
			data->terrain->Render_Terrain(*data->piece);
			return TRUE;
			break;
		case GDK_KEY_z:
			((Piece*)(*data->piece))->RotateRight();
			// Implémentation du 'Wall kick' si on essaye de tourner alors qu'il y a un obstacle
			if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
				((Piece*)(*data->piece))->Move(1, 0);
				if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
					((Piece*)(*data->piece))->Move(-2, 0);
					if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece))) {
						((Piece*)(*data->piece))->Move(1, 0);
						((Piece*)(*data->piece))->RotateLeft();
					}
				}
			}
			data->terrain->Render_Terrain(*data->piece);
			return TRUE;
			break;
		case GDK_KEY_Left:
			((Piece*)(*data->piece))->Move(-1, 0);
			if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece)))
				((Piece*)(*data->piece))->Move(1, 0);
			data->terrain->Render_Terrain(*data->piece);
			return TRUE;
			break;
		case GDK_KEY_Right:
			((Piece*)(*data->piece))->Move(1, 0);
			if (((Terrain*)data->terrain)->CheckCollision((Piece*)(*data->piece)))
				((Piece*)(*data->piece))->Move(-1, 0);
			data->terrain->Render_Terrain(*data->piece);
			return TRUE;
			break;
    }
	// Si l'évènement n'a pas été gérer, appeler la classe de base
    return Gtk::Window::on_key_press_event(event);
}


Gtk::Grid* GameWindow::GetGrid() const {
    return terrainGrid;
}