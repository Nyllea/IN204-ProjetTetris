#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "./Affichage-Gtk/TetrisElements_Graphic-Piece.hpp"
#include "./Affichage-Gtk/TetrisElements_Graphic-Terrain.hpp"
#include "TimeManager.hpp"

#include <glibmm.h>
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/cssprovider.h>
#include <gtkmm-3.0/gtkmm/overlay.h>
#include <gtkmm-3.0/gtkmm/window.h>

#define MAIN_LOOP_TIMEOUT 1000 // Game loop called every 1000ms = 1s
#define BACKGROUND_COLOR "rgba(100, 100, 100, 1.0)"
#define GAMEOVER_COLOR "rgba(255, 0, 0, 1.0)"
#define MAINMENU_COLOR "rgba(255, 0, 0, 1.0)"
#define GAMEOVER_BKG_COLOR "rgba(127, 127, 127, 0.75)"

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

  public:
	GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing);
	~GameWindow();

  private:
	Gtk::Overlay *overlay;

	// Pour le jeu
	sigc::connection keyboardControls, mainGameLoop;
	TerrainPiece terrainPiece;
	TimeManager timeManager;

	// Définitions des objets Gtk
	Gtk::Box *gameOverMenu, *mainMenu, *gameBoard, *pauseMenu;
	Gtk::Label *scoreLabelOverMenu, *bestScoreLabelOverMenu, *levelLabel, *scoreLabel, *bestScoreLabelPauseMenu, *bestScoreLabelMainMenu;
	Gtk::Label timeLabels[MAX_PREDICTION];

	Glib::RefPtr<Gtk::CssProvider> cssProvider;

	// Paramètres de jeu
	int bestScore;
	int currentMainLoopTimeout;
	bool isInGame;

	// Fonctions de création des différents menus et fenêtre de jeu Gtk
	Gtk::Box *MakeGameBoard(Gtk::Grid *terrainGrid, Gtk::Grid *previewGrid, Gtk::Grid *previousPreviewGrid);
	Gtk::Box *MakeMainMenu();
	Gtk::Box *MakeGameOverMenu();
	Gtk::Box *MakePauseMenu();

	// Fonctions des boutons de menu correpondants
	void ExitGame();
	void RestartGame();
	void MainMenuButton();
	void StartButton();
	void ResumeButton();
	void PauseButton();

	// Fonctions utiles pour la gestion de l'affichage des menus Gtk
	void HideAllGtkWidgets() const;
	void DisconnectGameControls();
	void ReconnectGameControls();
	void RenderScore(const bool isGameBoard) const;
	void ActualiseTimeStickers();

	// Boucle de jeu principale appellée une fois toute les CURRENT_MAIN_TIME_LOOP ms
	bool MainGameLoop();

	// Réinitialise le timeout de MainGameLoop
	void ResetMainGameLoop();

	// GameOver
	void GameOver();

	// Fonction de gestion des entrées clavier
	bool OnKeyPress(GdkEventKey *const event);
};

#endif