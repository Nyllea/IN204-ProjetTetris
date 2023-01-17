#include "TetrisElements_Graphic.hpp"

// Permet d'acceder à la couleur (getter pour m_color)
char PieceGraphic::GetColorChar() const {
    return (char)m_color;
}



// Convertit le code couleur(char) en une couleur GdkRGBA
GdkRGBA const* TerrainGraphic::CharToColor(const char colorVal) const {
    switch (colorVal) {
        case 1:
            return &red;
            break;
        case 2:
            return &green;
            break;
        case 3:
            return &blue;
            break;
        default:
            return &emptyColor;
            break;
    }
}

// Setter pour la grille graphique représentant le terrain
void TerrainGraphic::SetGrid(GtkWidget* const grid) {
    terrainGrid = grid;
}

// Remplissage de la grille graphique représentant le terrain avec des blocs vides
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

// Actualisation graphique du terrain
void TerrainGraphic::Render_Terrain(const PieceGraphic* const piece = NULL) {
    GtkWidget* block;
    GdkRGBA const* color;

    for (unsigned char i = 0; i < TERR_NBR_LINES; i++) {
    	for (unsigned char j = 0; j < TERR_NBR_COL; j++) {
            block = gtk_grid_get_child_at(GTK_GRID(terrainGrid), j, i);

            if (piece != NULL && ((Piece*)piece)->IsAt(j, i))
                color = CharToColor(piece->GetColorChar());
            else
                color = CharToColor(matrix[TERR_NBR_COL*i + j]);

            gtk_widget_override_background_color(block, GTK_STATE_FLAG_NORMAL, color);
        }
    }
}

// Ajoute la piece à la matrice du terrain
void TerrainGraphic::ImprintPiece(const PieceGraphic* const piece) {
    for (int i=0; i<PIECE_MAT_SIZE; i++) {
        for (int j=0; j<PIECE_MAT_SIZE; j++) {
            if ((*(Piece*)piece)(i, j) == 1) {
                char terrainCoord = ((Piece*)piece)->ToTerrainCoord(j, i);
                if (terrainCoord < 0) // Si les coordonnées pointent en dehors du terrain
                    continue;

                matrix[terrainCoord] = piece->GetColorChar();
            }
        }
    }
}