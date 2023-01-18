#ifndef TETRISELEMENTS_HPP
#define TETRISELEMENTS_HPP

#include <bitset>
#include <iostream>
#include <algorithm>

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10
#define PIECE_MAT_SIZE 5

class Piece {
public:
    enum Type { Square, I, L, LMirrored, N, NMirrored, T };
    signed char posX, posY;
private:
    Type m_type;
    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> matrix;


    // Storing the matrices for the different shapes
    /*
    00000
    00000
    00110
    00110
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> SquareMatrix{0x63000};
    /*
    00000
    00100
    00100
    00100
    00100 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> IMatrix{0x421080};
    /*
    00000
    00100
    00100
    00110
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> LMatrix{0x61080};
    /*
    00000
    00100
    00100
    01100
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> LMirrMatrix{0x31080};
    /*
    00000
    01000
    01100
    00100
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> NMatrix{0x21840};
    /*
    00000
    00010
    00110
    00100
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> NMirrMatrix{0x23100};
    /*
    00000
    00000
    01110
    00100
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> TMatrix{0x23800};

public:
    Piece(const Piece& source): m_type(source.m_type), matrix(source.matrix) {}
    Piece(const Type type);

    Piece& operator = (const Piece& source);

    // Permet d'accéder à la case (x,y) de la matrice de la pièce
    bool operator ()(const unsigned char x, const unsigned char y) const;

    void SetMaxHeight();
    void SetXPos(const signed char xPos);

    void RotateLeft();
    void RotateRight();

    // Vérifie si la pièce occupe la case (x,y) du terrain (en fonction de sa position et de sa matrice)
    bool IsAt(const unsigned char x, const unsigned char y) const;

    // Déplace la pièce de dirX selon X et de dirY selon Y
    void Move(const signed char dirX, const signed char dirY);

    // Retourne la position de la case (x,y) de la matrice de la pièce dans les coordonnées de la matrice du terrain (négatif si hors du terrain)
    char ToTerrainCoord(const unsigned char x, const unsigned char y) const;
};

class Terrain {
protected:
    char matrix[TERR_NBR_LINES*TERR_NBR_COL];

public:
    Terrain();

    // Permet d'accéder à la valeur(char) du bloc en position (x,y) du terrain 
    char operator ()(const unsigned char x, const unsigned char y) const;

    // Retourne vrai si la pièce est hors du terrain ou touche un block, faux sinon
    bool CheckCollision(const Piece* const piece) const;
};

#endif