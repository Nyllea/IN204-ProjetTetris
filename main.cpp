#include <iostream>

#include "TetrisElements_Graphic.hpp"
#include "TetrisElements.hpp"
#include "GTK-Setup.hpp"
#include "Random.hpp"

#define WINDOW_NAME "Tetris"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define WINDOW_BORDER 5

#define GRID_SPACING 5

#define SPAWN_POS (TERR_NBR_COL / 2) - (PIECE_MAT_SIZE / 2)

struct TerrainPiece {
	TerrainGraphic* terrain;
	PieceGraphic** piece;
};


void PrintPiece(Piece& piece) {
	for (int i = 0; i < PIECE_MAT_SIZE; i++) {
    	for (int j = 0; j < PIECE_MAT_SIZE; j++) {
            std::cout << piece(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

void PrintTerrain(Terrain& terrain) {
	for (int i = 0; i < TERR_NBR_LINES; i++) {
    	for (int j = 0; j < TERR_NBR_COL; j++) {
            std::cout << (int)terrain(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

PieceGraphic* SpawnPiece() {
	PieceGraphic* piece = new PieceGraphic(GetRandomType(), GetRandomColor());

	((Piece*)piece)->SetMaxHeight();
	((Piece*)piece)->SetXPos(SPAWN_POS);

	return piece;
}

void ImprintPieceOnTerrain (TerrainGraphic* terrain, PieceGraphic* piece) {
	terrain->ImprintPiece(piece);

	delete(piece);
}

gboolean OnKeyPress (GtkWidget *widget, GdkEventKey *event, gpointer _data) {
	struct TerrainPiece* data = (struct TerrainPiece*) _data;

    switch(event->keyval){
		case GDK_KEY_space:
			ImprintPieceOnTerrain(data->terrain, *data->piece);
			(*data->piece) = SpawnPiece();
			data->terrain->Render_Terrain(*data->piece);
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
    return FALSE;
}

int main(int argc, char* argv[]) {
	TerrainGraphic terrain;
	PieceGraphic* currentPiece = SpawnPiece();

	// Initialise Gtk
    Gtk::Main app(argc, argv);

	// Generate an empty window with a GtkGrid (for the game terrain)
	GameWindow window(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER, GRID_SPACING);

	terrain.SetGrid(window.GetGrid());
	terrain.FillGrid(WINDOW_HEIGHT, WINDOW_WIDTH);
	
	// Enable Gtk to detect keyboard events
	//gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

	// Connect a keyboard input to a function
/* 	struct TerrainPiece* tp;
	tp->terrain = &terrain;
	tp->piece = &currentPiece;
	g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(OnKeyPress), tp); */

	//terrain.Render_Terrain(currentPiece);

	// Makes everything appear on-screen
	window.show_all();

    // Starts event loop running
	Gtk::Main::run(window);

    return 0;
}
