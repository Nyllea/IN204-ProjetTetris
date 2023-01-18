#include "GTK-Setup.hpp"


void SetupGrid(Gtk::Grid *grid, const guint spacing){
    grid->set_row_homogeneous(false);
    grid->set_column_homogeneous(false);

    grid->set_column_spacing(spacing);
    grid->set_row_spacing(spacing);

    //grid->set_name(name);

    grid->set_hexpand(true);
    grid->set_halign(Gtk::Align::ALIGN_CENTER);
    grid->set_vexpand(true);
    grid->set_valign(Gtk::Align::ALIGN_CENTER);
}

GameWindow::GameWindow (const Glib::ustring &name, const int width, const int height, const guint borderSize, const guint gridSpacing) {
    set_title(name);
    set_default_size(width, height);
    set_resizable(true);
    set_border_width(borderSize);
    set_position(Gtk::WIN_POS_CENTER);

    // Creation de la grille de jeu
    terrainGrid = Gtk::make_managed<Gtk::Grid>(); // Laisse Gtk delete le terrain quand la fenetre se ferme
    SetupGrid(terrainGrid, gridSpacing);
    add(*terrainGrid); //Ajout à la fenêtre
    //terrainGrid->show_all();
}

Gtk::Grid* GameWindow::GetGrid() const {
    return terrainGrid;
}