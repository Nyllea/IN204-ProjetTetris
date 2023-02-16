#ifndef GAME_OVER_MENU_HPP
#define GAME_OVER_MENU_HPP

#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/widget.h>
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/orientable.h>
#include <glibmm.h>

#define GAMEOVER_COLOR "rgba(255, 0, 0, 1.0)"

class GameOverMenu
{
public:
    GameOverMenu();

    inline Gtk::Widget &operator()() { return wrapper; };

public:
    Gtk::Box wrapper{Gtk::ORIENTATION_HORIZONTAL};

    Gtk::Label *gameOverLabel;

    Gtk::Button *retryBtn;
    Gtk::Button *mainMenuBtn;
    Gtk::Button *exitBtn;
};

#endif