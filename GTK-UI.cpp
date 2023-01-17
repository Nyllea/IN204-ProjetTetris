#include "GTK-UI.hpp"
#include <algorithm>

// event handler: ends main event loop (and program) when user clicks x in upper right
static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void createWindow(GtkWidget **window, const gchar* name, const gint width, const gint height, const gint borderSize) {
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(*window), name);
    gtk_window_set_default_size(GTK_WINDOW(*window), width, height);
    gtk_window_set_resizable (GTK_WINDOW(*window), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(*window), borderSize);

    g_signal_connect(*window, "destroy", G_CALLBACK(destroy), NULL);
}

void createGrid(GtkWidget **grid, const gchar *name, const guint spacing){
    *grid = gtk_grid_new ();

    gtk_grid_set_row_homogeneous(GTK_GRID(*grid), FALSE);
    gtk_grid_set_column_homogeneous(GTK_GRID(*grid), FALSE);

    //gtk_container_set_border_width(GTK_CONTAINER (*grid), 150);
    gtk_grid_set_column_spacing(GTK_GRID(*grid), spacing);
    gtk_grid_set_row_spacing(GTK_GRID(*grid), spacing);

    gtk_widget_set_name(*grid, name);
    //g_object_set (*grid, "margin", 22, NULL);

    gtk_widget_set_hexpand(*grid, TRUE);
    gtk_widget_set_halign(*grid, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(*grid, TRUE);
    gtk_widget_set_valign(*grid, GTK_ALIGN_CENTER);
}



GtkWidget* Setup_Terrain(int argc, char* argv[], TerrainGraphic *terrainGraph, const gint windowHeight, const gint windowWidth, const gint windowBorder, const gint blockSpacing) { // argc, argv used specially by gtk
    // GTK widgets used
    GtkWidget *window, *grid;

    // Necessary setup function
    gtk_init(&argc, &argv);

    // Create window widget
    createWindow(&window, "Tetris", windowWidth, windowHeight, windowBorder);

    // Create and add the terrain grid to the main window
    createGrid(&grid, "Terrain", blockSpacing);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Adds the grid to the terrain graphic object and fill it with the blocks
    terrainGraph->terrainGrid = grid;
    terrainGraph->FillGrid(windowWidth, windowHeight);
    
    return window;
}