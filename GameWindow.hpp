#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "TetrisElements_Graphic.hpp"

#include <gtkmm-3.0/gtkmm/window.h>

struct TerrainPiece {
	TerrainGraphic* terrain;
	PieceGraphic** piece;
};

class GameWindow : public Gtk::Window {
    private:
        Gtk::Grid* terrainGrid;

        bool OnKeyPress (GdkEventKey* const event, const TerrainPiece* data);

    public:
        GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing, const TerrainPiece* tp);

        Gtk::Grid* GetGrid() const;
};

#endif