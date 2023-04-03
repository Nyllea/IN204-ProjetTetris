#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "TetrisElements_Graphic.hpp"

#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/overlay.h>
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <glibmm.h>

#define MAIN_LOOP_TIMEOUT 1000 // Game loop called every 1000ms = 1s
#define BACKGROUND_COLOR "rgba(100, 100, 100, 1.0)"
#define GAMEOVER_COLOR "rgba(255, 0, 0, 1.0)"
#define MAINMENU_COLOR "rgba(255, 0, 0, 1.0)"

struct TerrainPiece
{
    TerrainGraphic *terrainGraph;
    PieceGraphic **pieceGraph;
};

class GameWindow : public Gtk::Window
{
private:
    Gtk::Overlay *overlay;

    // For the game
    Gtk::Grid *terrainGrid;
    sigc::connection keyboardControls, mainGameLoop;
    const TerrainPiece *terrainPiece;

    Gtk::Box *gameOverMenu, *mainMenu;

    bool OnKeyPress(GdkEventKey *const event, const TerrainPiece *data);

    bool MainGameLoop(const TerrainPiece *data);

    void ExitGame();
    void RestartGame();
    void MainMenuButton();
    void StartButton();

    Gtk::Box *MakeMainMenu();
    Gtk::Box *MakeGameOverMenu();

    void HideAll();
    void DisconnectGameControls();
    void ReconnectGameControls();

    void GameOver();

public:
    GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing, const TerrainPiece *tp);

    ~GameWindow();

    Gtk::Grid *GetGrid() const;
};

#endif