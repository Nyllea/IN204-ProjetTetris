#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "TetrisElements_Graphic.hpp"

#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/overlay.h>
#include <glibmm.h>

#define MAIN_LOOP_TIMEOUT 1000 // Game loop called every 1000ms = 1s
#define GAMEOVER_COLOR "rgba(255, 0, 0, 1.0)"

struct TerrainPiece
{
    TerrainGraphic *terrainGraph;
    PieceGraphic **pieceGraph;
};

class GameWindow : public Gtk::Window
{
private:
    Gtk::Grid *terrainGrid;
    Gtk::Overlay *overlay;
    sigc::connection keyboardControls, mainGameLoop;

    bool OnKeyPress(GdkEventKey *const event, const TerrainPiece *data);

    bool MainGameLoop(const TerrainPiece *data);

    void GameOver();

public:
    GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing, const TerrainPiece *tp);

    Gtk::Grid *GetGrid() const;
};

#endif