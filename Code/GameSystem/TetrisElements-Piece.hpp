#ifndef TETRIS_ELEMENTS_PIECE_HPP
#define TETRIS_ELEMENTS_PIECE_HPP

// Taille de la matrice représentant la pièce
#define PIECE_MAT_SIZE 5

#include <bitset>
#include <iostream>

#include "TetrisElements-Terrain.hpp"

class Piece
{
  public:
	enum Type
	{
		Square,
		I,
		L,
		LMirrored,
		N,
		NMirrored,
		T
	};

	Piece(const Type type);
	Piece(const Piece &source) : m_type(source.m_type), matrix(source.matrix), posX(source.posX), posY(source.posY){};

  private:
	// Type de la pièce
	Type m_type;

	// Matrice représentant la pièce (1 si la pièce occupe cette position, 0 sinon)
	std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> matrix;

	// Coordonnées sur le terrain du bloc en haut à gauche de la matrice de la pièce
	signed char posX, posY;

	// Stockage des matrices des différentes pièces
	/*
	00000
	00000
	00110
	00110
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> SquareMatrix{0x63000};
	/*
	00000
	00100
	00100
	00100
	00100 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> IMatrix{0x421080};
	/*
	00000
	00100
	00100
	00110
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> LMatrix{0x61080};
	/*
	00000
	00100
	00100
	01100
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> LMirrMatrix{0x31080};
	/*
	00000
	01000
	01100
	00100
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> NMatrix{0x21840};
	/*
	00000
	00010
	00110
	00100
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> NMirrMatrix{0x23100};
	/*
	00000
	00000
	01110
	00100
	00000 */
	static constexpr std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> TMatrix{0x23800};

  public:
	static inline int GetPieceMatSize() { return PIECE_MAT_SIZE; };

  public:
	// Permet d'accéder à la case (x,y) de la matrice de la pièce
	bool operator()(const unsigned char x, const unsigned char y) const;

	// Modifie la position Y de la pièce pour la placer en haut du terrain
	void SetMaxHeight();

	// Assigne la position X de la pièce
	void SetXPos(const signed char xPos);

	// Tourne la pièce de 90 degrés vers la gauche (sens anti-horaire)
	void RotateLeft();

	// Tourne la pièce de 90 degrés vers la droite (sens horaire)
	void RotateRight();

	// Si useTerrainPosition, vérifie si la pièce occupe la case (x,y) du terrain (en fonction de sa position et de sa matrice)
	// Sinon vérifie si la case (x,y) de la matrice est occupée
	bool IsAt(const unsigned char x, const unsigned char y, const bool useTerrainPosition) const;

	// Déplace la pièce de dirX selon X et de dirY selon Y
	void Move(const signed char dirX, const signed char dirY);

	// Retourne la position de la case (x,y) de la matrice de la pièce dans les coordonnées de la matrice du terrain (négatif si hors du terrain)
	short int ToTerrainCoord(const unsigned char x, const unsigned char y) const;
};

#endif