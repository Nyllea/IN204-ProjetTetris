#ifndef TETRIS_ELEMENTS_TERRAIN_HPP
#define TETRIS_ELEMENTS_TERRAIN_HPP

#include <bitset>
#include <iostream>

#include "TetrisElements-Piece.hpp"

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10

// Forward declaration pour éviter les erreurs 'Piece has not been declared'
class Piece;

class Terrain
{
  protected:
	// Matrice représentant le terrain (0 si la case est vide, une valeur représentant la couleur sinon)
	char matrix[TERR_NBR_LINES * TERR_NBR_COL];

	// Pour le calcul du score
	int score = 0;
	int clearedLines = 0;

	// Réinitialise le terrain
	void ResetMatrix();

	// Réinitialise le score
	void ResetScore();

	// Vérifie si la ligne contenant le bloc en terrainCoord est complète
	bool CheckLine(const short int terrainCoord) const;

	// Retire la ligne contenant le bloc en terrainCoord
	void RemoveLine(const int teerainCoord);

  public:
	Terrain();

	static inline int GetNbrLines() { return TERR_NBR_LINES; };
	static inline int GetNbrCol() { return TERR_NBR_COL; };

	// Retourne vrai si la pièce est hors du terrain ou touche un bloc, faux sinon
	bool CheckCollision(const Piece *const piece) const;

	// Retourne le nombre de lignes complétées
	int GetClearedLines() const;

	// Retourne le score
	int GetScore() const;

	// Mets à jour le score et le nombre de lignes complétées
	void UpdateScore(int numberline);
};

#endif