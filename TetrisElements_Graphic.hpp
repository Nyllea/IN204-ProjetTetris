#ifndef TETRISELEMENTS_GRAPHIC_HPP
#define TETRISELEMENTS_GRAPHIC_HPP

#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>
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
        Gtk::Grid *terrainGrid;

        // Valeur des couleurs
        static constexpr char emptyColor[] = "rgba(168, 168, 168, 1.0)";
        static constexpr char red[] = "rgba(255, 0, 0, 1.0f)";
        static constexpr char green[] = "rgba(0, 255, 0, 1.0)";
        static constexpr char blue[] = "rgba(0, 0, 255, 1.0f)";

        // Convertit le code couleur(char) en une couleur GdkRGBA
        Gdk::RGBA CharToColor(const char colorVal) const;

    public:
        TerrainGraphic() : Terrain() {};
        TerrainGraphic(Terrain terrain) : Terrain(terrain) {}

        // Setter pour la grille graphique représentant le terrain
        void SetGrid(Gtk::Grid* const grid);

        // Remplissage de la grille graphique représentant le terrain avec des blocs vides
        void FillGrid(const int windowHeight, const int windowWidth);

        // Actualisation graphique du terrain
        void Render_Terrain(const PieceGraphic* const piece);

        // Ajoute la piece à la matrice du terrain
        void ImprintPiece(const PieceGraphic* const piece);
};

#endif