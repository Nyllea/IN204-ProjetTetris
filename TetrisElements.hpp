#ifndef TETRISELEMENT_HPP
#define TETRISELEMENT_HPP

#include <bitset>
#include <iostream>

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10
#define PIECE_MAT_SIZE 5

class Piece {
public:
    enum Type { Square, I, L, LMirrored, N, NMirrored, T };

private:
    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> matrix;
    Type m_type;

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
    01000
    01000
    01100
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> LMatrix{0x30840};
    /*
    00000
    00010
    00010
    00110
    00000 */
    static constexpr std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> LMirrMatrix{0x62100};
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
    Piece(Type type);

    Piece& operator = (const Piece& source);
    bool operator ()(const int x, const int y) const;

    void RotateLeft();
    void RotateRight();    
};

class Terrain {
private:
    char matrix[TERR_NBR_LINES*TERR_NBR_COL];

public:
    Terrain();

    char operator ()(const int x, const int y) const;
};

#endif