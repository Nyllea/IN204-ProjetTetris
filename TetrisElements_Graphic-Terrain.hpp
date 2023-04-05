#ifndef TETRISELEMENTS_GRAPHIC_TERRAIN_HPP
#define TETRISELEMENTS_GRAPHIC_TERRAIN_HPP

#include "TetrisElements-Terrain.hpp"
#include "TimeManager.hpp"

#include <cassert>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>

// Forward declaration pour éviter les erreurs de linker
class TimeManager;

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
  public:
	TerrainGraphic() : Terrain(){};
	TerrainGraphic(const Terrain terrain) : Terrain(terrain){};

	// Remplissage de le grille graphique représentant le terrain avec des blocs vides
	void FillGrid(const int windowHeight, const int windowWidth) { GridGraphic::FillGrid(windowHeight, windowWidth, TERR_NBR_LINES, TERR_NBR_COL); };

	// Actualisation graphique du terrain
	void RenderGrid(const PieceGraphic *const piece) { GridGraphic::RenderGrid(piece, TERR_NBR_LINES, TERR_NBR_COL, matrix); };

	// Réinitialise le terrain avec une nouvelle pièce
	void ResetTerrain(PieceGraphic **const piece, TimeManager &timeManager);

	// Ajoute la piece à la matrice du terrain
	// Si c'est une pièce dans le présent, enlève les lignes complètes et génère une nouvelle piece
	// Si c'est une pièce dans le futur, l'ajoute aux pièces placées et génère une nouvelle pièce
	// Retourne false s'il y a eu une collision
	bool ImprintPiece(PieceGraphic **const piece, TimeManager &timeManager);

	// Ajoute la piece à la matrice du terrain et retourne false s'il y a une collision
	bool ImprintFuturePieces(const std::list<PieceGraphic *> &piece);

	// Enlève la piece de la matrice du terrain et affiche le terrain
	void DeprintFuturePieces(const std::list<PieceGraphic *> &piece);
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