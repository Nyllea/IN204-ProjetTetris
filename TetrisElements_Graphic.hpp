#ifndef TETRISELEMENTS_GRAPHIC_HPP
#define TETRISELEMENTS_GRAPHIC_HPP

#include "TetrisElements.hpp"

#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>

#define SPAWN_POS (TERR_NBR_COL / 2) - (PIECE_MAT_SIZE / 2)

class PieceGraphic : Piece
{
  public:
	enum Color
	{
		Grey = 0,
		Red = 1,
		Green = 2,
		Blue = 3
	};

  private:
	Color m_color;

  public:
	PieceGraphic(const PieceGraphic &source) : Piece((Piece)source), m_color(source.m_color) {}

	PieceGraphic(const Piece *const piece, const Color color) : Piece(*piece), m_color(color) {}
	PieceGraphic(const Type type, const Color color) : Piece(type), m_color(color) {}

	// Permet d'acceder à la couleur (getter pour m_color)
	char GetColorChar() const;
};

#include "Random.hpp" // Doit etre mis ici pour que PieceGraphic soit defini quand on importe Random.hpp

class GridGraphic
{
  protected:
	Gtk::Grid *grid;

	// Valeur des couleurs
	static constexpr char emptyColor[] = "rgba(168, 168, 168, 1.0)";
	static constexpr char red[] = "rgba(255, 0, 50, 1.0)";
	static constexpr char green[] = "rgba(50, 255, 0, 1.0)";
	static constexpr char blue[] = "rgba(0, 50, 255, 1.0)";

	// Convertit le code couleur(char) en une couleur GdkRGBA
	Gdk::RGBA CharToColor(const char colorVal) const;

	// Remplissage de la grille graphique avec des blocs vides
	void FillGrid(const int windowHeight, const int windowWidth, const int lineNbr, const int colNbr);

	// Actualisation graphique de la grille
	void RenderGrid(const PieceGraphic *const piece, const int lineNbr, const int colNbr, const char *matrix = NULL);

  public:
	// Setter pour la grille graphique
	void SetGrid(Gtk::Grid *const grid);
};

class TerrainGraphic : public Terrain, public GridGraphic
{
  private:
	// Supprime l'ancienne pièce et en génère une nouvelle
	void SpawnRandomPiece(PieceGraphic **const piece);

  public:
	TerrainGraphic() : Terrain(){};
	TerrainGraphic(const Terrain terrain) : Terrain(terrain){};

	// Remplissage de le grille graphique représentant le terrain avec des blocs vides
	void FillGrid(const int windowHeight, const int windowWidth) { GridGraphic::FillGrid(windowHeight, windowWidth, TERR_NBR_LINES, TERR_NBR_COL); };

	// Actualisation graphique du terrain
	void RenderGrid(const PieceGraphic *const piece) { GridGraphic::RenderGrid(piece, TERR_NBR_LINES, TERR_NBR_COL, matrix); };

	// Réinitialise le terrain avec une nouvelle pièce
	void ResetTerrain(PieceGraphic **const piece);

	// Instancie une piece sur le terrain et en retourne une reference
	PieceGraphic *CreateRandomPiece() const;

	// Ajoute la piece à la matrice du terrain
	void ImprintPiece(PieceGraphic **const piece);
};

class PreviewGraphic : public GridGraphic
{
  public:
	// Remplissage de la grille graphique représentant la prochaine pièce avec des blocs vides
	void FillGrid(const int windowHeight, const int windowWidth) { GridGraphic::FillGrid(windowHeight, windowWidth, PIECE_MAT_SIZE, PIECE_MAT_SIZE); };

	// Actualisation graphique de la preview
	void RenderGrid(const PieceGraphic *const piece) { GridGraphic::RenderGrid(piece, PIECE_MAT_SIZE, PIECE_MAT_SIZE); };
};

#endif