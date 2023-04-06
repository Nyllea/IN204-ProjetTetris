#include "GameWindow.hpp"

#include <gtkmm-3.0/gtkmm/main.h>

#define WINDOW_NAME "Time Tetris"
#define WINDOW_WIDTH 1422
#define WINDOW_HEIGHT 800
#define WINDOW_BORDER 0

#define GRID_SPACING 1

int main(int argc, char *argv[])
{
	// Initialise Gtk
	Gtk::Main app(argc, argv);

	// Instancie la fenêtre de jeu
	GameWindow gWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BORDER, GRID_SPACING);

	// Affiche tous les widgets Gtk de la fenêtre
	gWindow.show_all();

	// Lance la boucle principale de la fenêtre Gtk
	Gtk::Main::run(gWindow);

	return 0;
}
