#ifndef GTK_UI_HPP
#define GTK_UI_HPP

#include <algorithm>

#include <gtkmm-3.0/gtkmm/main.h>
#include <gtkmm-3.0/gtkmm/window.h>

#include "TetrisElements_Graphic.hpp"

class GameWindow : public Gtk::Window {
    private:
        Gtk::Grid* terrainGrid;

    public:
        GameWindow(const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing);

        Gtk::Grid* GetGrid() const;
};

#endif