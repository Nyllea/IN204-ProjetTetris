#ifndef TETRISELEMENTS_GRAPHIC_HPP
#define TETRISELEMENTS_GRAPHIC_HPP

#include <gtk-3.0/gtk/gtk.h>
#include "TetrisElements.hpp"

class PieceGraphic : Piece {
public:
    enum Color { Grey = 0, Red = 1, Green = 2, Blue = 3 };

private:
    Color m_color;

public:
    PieceGraphic(const PieceGraphic& source) : Piece((Piece)source), m_color(source.m_color) {}

    PieceGraphic(const Piece* const piece, const Color color) : Piece(*piece), m_color(color) {}
    PieceGraphic(const Type type, const Color color) : Piece(type), m_color(color) {}

    // Permet d'acceder à la couleur (getter pour m_color)
    char GetColorChar() const;

};

class TerrainGraphic : Terrain {
private:
    GtkWidget *terrainGrid;

    // Valeur des couleurs
    static constexpr GdkRGBA emptyColor = { 0.66f, 0.66f, 0.66f, 1.0f };
    static constexpr GdkRGBA red = { 1.0f, 0, 0, 1.0f };
    static constexpr GdkRGBA green = { 0, 1.0f, 0, 1.0f };
    static constexpr GdkRGBA blue = { 0, 0, 1.0f, 1.0f };

    // Convertit le code couleur(char) en une couleur GdkRGBA
    GdkRGBA const* CharToColor(const char colorVal) const;

public:
    TerrainGraphic() : Terrain() {};
    TerrainGraphic(Terrain terrain) : Terrain(terrain) {}

    // Setter pour la grille graphique représentant le terrain
    void SetGrid(GtkWidget* const grid);

    // Remplissage de la grille graphique représentant le terrain avec des blocs vides
    void FillGrid(const int windowHeight, const int windowWidth);

    // Actualisation graphique du terrain
    void Render_Terrain(const PieceGraphic* const piece);

    // Ajoute la piece à la matrice du terrain
    void ImprintPiece(const PieceGraphic* const piece);
};

#endif