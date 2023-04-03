#include "GameWindow.hpp"
#include "TetrisElements_Graphic.hpp"

#include <gtkmm-3.0/gtkmm/main.h>

#define WINDOW_NAME "Tetris"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define WINDOW_BORDER 5

#define GRID_SPACING 1

// To help with debugging
/*
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
*/

int main(int argc, char *argv[])
{
	// Initialise Gtk
	Gtk::Main app(argc, argv);

	// Generate an empty window with a GtkGrid (for the game terrain)
	GameWindow gWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER, GRID_SPACING);

	// Makes everything appear on-screen
	gWindow.show_all();

	// Starts event loop running
	Gtk::Main::run(gWindow);

	return 0;
}
