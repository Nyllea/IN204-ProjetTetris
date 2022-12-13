#include <bitset>
#include <iostream>

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10
#define PIECE_MAT_SIZE 5

/*
template<class T, size_t n, size_t m>
void Copy(T source[n][m], T dest[n][m]) {
    for (int line=0; line<n; line++) {
        for (int col=0; col<m; col++) {
            dest[line][col] = source[line][col];
        }
    }
}
*/

class Piece {
public:
    enum Type { Square, I, L, LMirrored, N, NMirrored, T };

private:
    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> matrix;
    Type m_type;

/*
    Piece(Type type, std::initializer_list<int[PIECE_MAT_SIZE]> values) {
        m_type = type;
        
        for (int i =0; i<PIECE_MAT_SIZE; i++) {
            for (int j=0; j<PIECE_MAT_SIZE; j++) {
                matrix.set(PIECE_MAT_SIZE*i + j, *(values.begin() + i)[j]);
            }
        }
    }
*/

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
    
    Piece& operator = (const Piece& source) {
        m_type = source.m_type;
        matrix = source.matrix;
        return *this;
    }

    Piece(Type type) {
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

    bool operator ()(const int x, const int y) const
    {
        if (x>=0 && x<PIECE_MAT_SIZE && y>=0 && y<PIECE_MAT_SIZE)
            return matrix[PIECE_MAT_SIZE*x + y];
        else {
            std::cout << "Error - Piece matrix index out of bounds" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void RotateLeft() {
        if (m_type == Square)
            return;

        std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> temp(matrix);

        for (int i = 0; i < PIECE_MAT_SIZE; i++) {
            for (int j = 0; j < PIECE_MAT_SIZE; j++) {
                matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*(4 - j) + i];
            }
        }
    }

    void RotateRight() {
        if (m_type == Square)
            return;

        std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> temp(matrix);

        for (int i = 0; i < PIECE_MAT_SIZE; i++) {
            for (int j = 0; j < PIECE_MAT_SIZE; j++) {
                matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*j + (4 - i)];
            }
        }
    }
};

class Terrain {
private:
    char matrix[TERR_NBR_LINES*TERR_NBR_COL];

public:
    Terrain() {
        for (int i=0; i<TERR_NBR_LINES; i++) {
            for (int j=0; j<TERR_NBR_COL; j++) {
                matrix[TERR_NBR_COL*i + j] = 0;
            }
        }
    }

    char operator ()(const int x, const int y) const
    {
        if (x>=0 && x<TERR_NBR_LINES && y>=0 && y<TERR_NBR_COL)
            return matrix[TERR_NBR_COL*x + y];
        else {
            std::cout << "Error - Terrain matrix index out of bounds" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};