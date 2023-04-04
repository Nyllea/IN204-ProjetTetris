#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "PiecesManager.hpp"
#include "TetrisElements_Graphic-Piece.hpp"
#include "TetrisElements_Graphic-Terrain.hpp"

#include <deque>
#include <glibmm.h>
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/overlay.h>
#include <gtkmm-3.0/gtkmm/window.h>

#define MAIN_LOOP_TIMEOUT 1000 // Game loop called every 1000ms = 1s
#define BACKGROUND_COLOR "rgba(100, 100, 100, 1.0)"
#define GAMEOVER_COLOR "rgba(255, 0, 0, 1.0)"
#define MAINMENU_COLOR "rgba(255, 0, 0, 1.0)"

struct TerrainPiece
{
	TerrainGraphic *terrainGraph;
	PreviewGraphic *previewGraph;
	PreviewGraphic *previousPreviewGraph;
	PieceGraphic **pieceGraph;

	void SetTerrainPiece(TerrainGraphic *_terrainGraph, PreviewGraphic *_previewGraph, PreviewGraphic *_previousPreviewGraph, PieceGraphic **_pieceGraph)
	{
		terrainGraph = _terrainGraph;
		previewGraph = _previewGraph;
		previousPreviewGraph = _previousPreviewGraph;
		pieceGraph = _pieceGraph;
	};
};

class GameWindow : public Gtk::Window
{
  private:
	Gtk::Overlay *overlay;

	// For the game
	sigc::connection keyboardControls, mainGameLoop;
	TerrainPiece terrainPiece;
	PiecesManager piecesManager;

	Gtk::Box *gameOverMenu, *mainMenu, *gameBoard;
	Gtk::Label *scoreValue;

	bool OnKeyPress(GdkEventKey *const event);

	bool MainGameLoop();

	void ExitGame();
	void RestartGame();
	void MainMenuButton();
	void StartButton();

	Gtk::Box *MakeGameBoard(Gtk::Grid *terrainGrid, Gtk::Grid *previewGrid, Gtk::Grid *previousPreviewGrid, Gtk::Label *scoreValue);
	Gtk::Box *MakeMainMenu();
	Gtk::Box *MakeGameOverMenu();

	void HideAll();
	void DisconnectGameControls();
	void ReconnectGameControls();

	void GameOver();

  public:
	GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing);

	~GameWindow();
};

#endif