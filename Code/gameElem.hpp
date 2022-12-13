

#define TERR_NBR_LINES 22
#define TERR_NBR_COL 10
#define PIECE_MAT_SIZE 5

enum PieceType { Square, I, L, LMirrored, N, NMirrored, T };

class Piece {
private:
    bool matrix[PIECE_MAT_SIZE][PIECE_MAT_SIZE];

public:
    Piece(PieceType type) {
        switch (type)
        {
        case Square:
            
            break;
        case I:

            break;
        case L:

            break;
        case LMirrored:

            break;
        case N:
            
            break;
        case NMirrored:

            break;
        case T:

            break;
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