#include <iostream>

#include "TetrisElements.hpp"
#include "GTK-UI.hpp"

void PrintPiece(Piece piece) {
	for (int i = 0; i < PIECE_MAT_SIZE; i++) {
    	for (int j = 0; j < PIECE_MAT_SIZE; j++) {
            std::cout << piece(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

void PrintTerrain(Terrain terrain) {
	for (int i = 0; i < TERR_NBR_LINES; i++) {
    	for (int j = 0; j < TERR_NBR_COL; j++) {
            std::cout << (int)terrain(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

gboolean OnKeyPress (GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_space){
        printf("SPACE KEY PRESSED!\n");
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char* argv[]) {
	/* std::cout << "Tet" << std::endl;

	Piece squa(Piece::Type::N);

	PrintPiece(squa);

	squa.RotateLeft();

	PrintPiece(squa);

	squa.RotateRight();

	PrintPiece(squa);

	Terrain terrain;

	PrintTerrain(terrain); */

	TerrainGraphic terrain;

	// Generate an empty terrain in window and store the GtkGrid representing it in terrain
	GtkWidget* window = Setup_Terrain(argc, argv, &terrain, 1000, 800, 5, 5);
	
	// Enable Gtk to detect keyboard events
	gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

	// Connect a keyboard input to a function
	g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(OnKeyPress), NULL);

	// Makes it appear on-screen
    gtk_widget_show_all(window);

    // Starts event loop running
    gtk_main();

    return 0;
}
