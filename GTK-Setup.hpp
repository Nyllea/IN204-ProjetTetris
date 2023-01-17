#ifndef GTK_UI_HPP
#define GTK_UI_HPP

#include <algorithm>
#include <gtk-3.0/gtk/gtk.h>
#include "TetrisElements_Graphic.hpp"

// Initialize Gtk and return a window containing the terrain grid and store the GtkGrid representing it in terrainGraph
GtkWidget* Setup_Terrain(int argc, char* argv[], 
    TerrainGraphic *terrainGraph, const gint windowHeight, const gint windowWidth, const gint windowBorder, const gint blockSpacing, 
    PieceGraphic** currentPiecePtr);

#endif