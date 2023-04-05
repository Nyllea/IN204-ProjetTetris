#ifndef TETRIS_ELEMENTS_TERRAIN_HPP
#define TETRIS_ELEMENTS_TERRAIN_HPP

#include <bitset>
#include <iostream>

#include "TetrisElements-Piece.hpp"

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10

class Piece;

class Terrain
{
  protected:
	char matrix[TERR_NBR_LINES * TERR_NBR_COL];

	// Réinitialise le terrain
	void ResetMatrix();
	void ResetScore();

	// Vérifie si la ligne contenant le bloc en terrainCoord est complète
	bool CheckLine(const short int terrainCoord);

	// Retire la ligne contenant le bloc en terrainCoord
	void RemoveLine(const int teerainCoord);


	int score=0;
	int cleared_lines=0;

  public:
	Terrain();

	static inline int GetNbrLines() { return TERR_NBR_LINES; };
	static inline int GetNbrCol() { return TERR_NBR_COL; };

	// Permet d'accéder à la valeur(char) du bloc en position (x,y) du terrain
	char operator()(const unsigned char x, const unsigned char y) const;

	// Retourne vrai si la pièce est hors du terrain ou touche un bloc, faux sinon
	bool CheckCollision(const Piece *const piece) const;

	//permet de récupérer la valeur du score ou le mettre a jour
	int get_cleared_lines();
	int get_score();
	void updatescore(int numberline);
	
	int get_info(int type);
	
};

#endif