#include <iostream>

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10
#define PIECE_MAT_SIZE 5

template<class T, size_t n, size_t m>
void Copy(T source[n][m], T dest[n][m]) {
    for (int line=0; line<n; line++) {
        for (int col=0; col<m; col++) {
            dest[line][col] = source[line][col];
        }
    }
}

enum PieceType { Square, I, L, LMirrored, N, NMirrored, T };

class Piece {
private:
    bool matrix[PIECE_MAT_SIZE][PIECE_MAT_SIZE];
    char orientation; // 0 = pointe vers le haut, 1 = droite, 2 = bas, 3 = 

public:
    Piece(PieceType type) {
        switch (type)
        {
        case Square:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case I:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case L:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case LMirrored:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case N:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0},
                {0, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case NMirrored:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 1, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        case T:
        {
            bool temp[PIECE_MAT_SIZE][PIECE_MAT_SIZE] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 0, 1, 0, 0},
                {0, 0, 0, 0, 0}};
            
            Copy<bool, PIECE_MAT_SIZE, PIECE_MAT_SIZE>(temp, matrix);
            break;
        }
        default:
            std::cout << "Error - Not a recognized piece type" << std::endl;
            exit(EXIT_FAILURE);
            break;
        }
    }
};

class Terrain {
private:
    char matrix[TERR_NBR_LINES][TERR_NBR_COL];

public:
    Terrain() {
        for (int i=0; i<TERR_NBR_LINES; i++) {
            for (int j=0; j<TERR_NBR_COL; j++) {
                matrix[i][j] = 0;
            }
        }
    }
};