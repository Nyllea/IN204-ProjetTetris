#include "TetrisElements.hpp"

Piece& Piece::operator = (const Piece& source) {
    m_type = source.m_type;
    matrix = source.matrix;
    return *this;
}

Piece::Piece(const Type type, const Color color) {
    m_type = type;
    m_color = color;

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

bool Piece::operator ()(const unsigned char x, const unsigned char y) const
{
    if (x<PIECE_MAT_SIZE && y<PIECE_MAT_SIZE)
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

    for (unsigned char i = 0; i < PIECE_MAT_SIZE; i++) {
        for (unsigned char j = 0; j < PIECE_MAT_SIZE; j++) {
            matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*(4 - j) + i];
        }
    }
}

void Piece::RotateRight() {
    if (m_type == Square)
        return;

    std::bitset<PIECE_MAT_SIZE*PIECE_MAT_SIZE> temp(matrix);

    for (unsigned char i = 0; i < PIECE_MAT_SIZE; i++) {
        for (unsigned char j = 0; j < PIECE_MAT_SIZE; j++) {
            matrix[PIECE_MAT_SIZE*i + j] = temp[PIECE_MAT_SIZE*j + (4 - i)];
        }
    }
}

bool Piece::IsAt(const unsigned char x, const unsigned char y) const {
    if (y < posY || y >= posY + PIECE_MAT_SIZE || x < posX || x >= PIECE_MAT_SIZE)
        return false;

    return matrix[(y - posY) * PIECE_MAT_SIZE + (x - posX)] == 1;
    // Idée: utiliser des opérations sur les bits directement
    //return (matrix & (1<<(PIECE_MAT_SIZE*PIECE_MAT_SIZE - (y - posY) * PIECE_MAT_SIZE + (x - posX)))) == 0;
}

void Piece::Move(const char dirX, const char dirY) {
    posX += dirX;
    posY += dirY;
}

char Piece::GetColorChar() const {
    return (char)m_color;
}


Terrain::Terrain() {
    for (int i=0; i<TERR_NBR_LINES; i++) {
        for (int j=0; j<TERR_NBR_COL; j++) {
            matrix[TERR_NBR_COL*i + j] = 0;
        }
    }
}

char Terrain::operator ()(const unsigned char x, const unsigned char y) const
{
    if (x<TERR_NBR_LINES && y<TERR_NBR_COL)
        return matrix[TERR_NBR_COL*x + y];
    else {
        std::cout << "Error - Terrain matrix index out of bounds" << std::endl;
        exit(EXIT_FAILURE);
    }
}

GdkRGBA const* TerrainGraphic::CharToColor(const char colorVal) const {
    switch (colorVal) {
        case 1:
            return &red;
            break;
        default:
            return &emptyColor;
            break;
    }
}

void TerrainGraphic::FillGrid(const int windowHeight, const int windowWidth) {
    gint blockSize = std::min(windowWidth, windowHeight) / std::max(TERR_NBR_LINES, TERR_NBR_COL);

    for (int i = 0; i < TERR_NBR_LINES; i++) {
    	for (int j = 0; j < TERR_NBR_COL; j++) {
            GtkWidget *temp = gtk_label_new ("");
            gtk_widget_override_background_color(temp, GTK_STATE_FLAG_NORMAL, &emptyColor);
            gtk_widget_set_size_request(temp, blockSize, blockSize);
            gtk_grid_attach(GTK_GRID(terrainGrid), temp, j, i, 1, 1);
        }
    }
}

void TerrainGraphic::Render_Terrain(const Piece* const piece = NULL) {
    GtkWidget* block;
    GdkRGBA const* color;

    for (unsigned char i = 0; i < TERR_NBR_LINES; i++) {
    	for (unsigned char j = 0; j < TERR_NBR_COL; j++) {
            block = gtk_grid_get_child_at(GTK_GRID(terrainGrid), j, i);

            if (piece != NULL && piece->IsAt(i, j))
                color = CharToColor(piece->GetColorChar());
            else
                color = CharToColor(matrix[TERR_NBR_COL*i + j]);

            gtk_widget_override_background_color(block, GTK_STATE_FLAG_NORMAL, color);
        }
    }
}