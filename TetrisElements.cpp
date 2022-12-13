#include "TetrisElements.hpp"

Piece& Piece::operator = (const Piece& source) {
    m_type = source.m_type;
    matrix = source.matrix;
    return *this;
}

Piece::Piece(Type type) {
    m_type = type;

    switch (type) {
        case Square:
            matrix = Piece::SquareMatrix;
            break;
        case I:
            matrix = Piece::IMatrix;
            break;
        case L:
            matrix = Piece::LMatrix;
            break;
        case LMirrored:
            matrix = Piece::LMirrMatrix;
            break;
        case N:
            matrix = Piece::NMatrix;
            break;
        case NMirrored:
            matrix = Piece::NMirrMatrix;
            break;
        case T:
            matrix = Piece::TMatrix;
            break;
        default:
            std::cout << "Error - Not a recognized piece type" << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
}

bool Piece::operator ()(const int x, const int y) const
{
    if (x>=0 && x<PIECE_MAT_SIZE && y>=0 && y<PIECE_MAT_SIZE)
        return matrix[PIECE_MAT_SIZE*x + y];
    else {
        std::cout << "Error - Piece matrix index out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Piece::RotateLeft() {
    if (m_type == Square)
        return;

    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> temp(matrix);

    for (int i = 0; i < PIECE_MAT_SIZE; i++) {
        for (int j = 0; j < PIECE_MAT_SIZE; j++) {
            matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*(4 - j) + i];
        }
    }
}

void Piece::RotateRight() {
    if (m_type == Square)
        return;

    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> temp(matrix);

    for (int i = 0; i < PIECE_MAT_SIZE; i++) {
        for (int j = 0; j < PIECE_MAT_SIZE; j++) {
            matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*j + (4 - i)];
        }
    }
}


Terrain::Terrain() {
    for (int i=0; i<TERR_NBR_LINES; i++) {
        for (int j=0; j<TERR_NBR_COL; j++) {
            matrix[TERR_NBR_COL*i + j] = 0;
        }
    }
}

char Terrain::operator ()(const int x, const int y) const
{
    if (x>=0 && x<TERR_NBR_LINES && y>=0 && y<TERR_NBR_COL)
        return matrix[TERR_NBR_COL*x + y];
    else {
        std::cout << "Error - Terrain matrix index out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }
}