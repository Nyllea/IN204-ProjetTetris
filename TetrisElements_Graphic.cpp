#include "TetrisElements_Graphic.hpp"

// Permet d'acceder à la couleur (getter pour m_color)
char PieceGraphic::GetColorChar() const
{
    return (char)m_color;
}

// Convertit le code couleur(char) en une couleur GdkRGBA
Gdk::RGBA TerrainGraphic::CharToColor(const char colorVal) const
{
    switch (colorVal)
    {
    case 1:
        return Gdk::RGBA(red);
        break;
    case 2:
        return Gdk::RGBA(green);
        break;
    case 3:
        return Gdk::RGBA(blue);
        break;
    default:
        return Gdk::RGBA(emptyColor);
        break;
    }
}

// Setter pour la grille graphique représentant le terrain
void TerrainGraphic::SetGrid(Gtk::Grid *const grid)
{
    terrainGrid = grid;
}

// Remplissage de la grille graphique représentant le terrain avec des blocs vides
void TerrainGraphic::FillGrid(const int windowHeight, const int windowWidth)
{
    gint blockSize = std::min(windowWidth, windowHeight) / std::max(TERR_NBR_LINES, TERR_NBR_COL);

    for (int i = 0; i < TERR_NBR_LINES; i++)
    {
        for (int j = 0; j < TERR_NBR_COL; j++)
        {
            Gtk::Label *temp = Gtk::make_managed<Gtk::Label>();
            temp->override_background_color(Gdk::RGBA(emptyColor), Gtk::STATE_FLAG_NORMAL);
            temp->set_size_request(blockSize, blockSize);
            // terrainGrid->add(*temp);
            terrainGrid->attach(*temp, j, i, 1, 1);
        }
    }
}

// Actualisation graphique du terrain
void TerrainGraphic::Render_Terrain(const PieceGraphic *const piece = NULL)
{
    Gtk::Widget *block;
    Gdk::RGBA color;

    for (unsigned char i = 0; i < TERR_NBR_LINES; i++)
    {
        for (unsigned char j = 0; j < TERR_NBR_COL; j++)
        {
            block = terrainGrid->get_child_at(j, i);

            if (piece != NULL && ((Piece *)piece)->IsAt(j, i))
                color = CharToColor(piece->GetColorChar());
            else
                color = CharToColor(matrix[TERR_NBR_COL * i + j]);

            block->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
        }
    }
}

// Instancie une piece sur le terrain et en retourne une reference
PieceGraphic *TerrainGraphic::SpawnPiece() const
{
    PieceGraphic *piece = new PieceGraphic(GetRandomType(), GetRandomColor());

    ((Piece *)piece)->SetMaxHeight();
    ((Piece *)piece)->SetXPos(SPAWN_POS);

    return piece;
}

// Ajoute la piece à la matrice du terrain, enlève les lignes complètes et génère une nouvelle piece
void TerrainGraphic::ImprintPiece(PieceGraphic **const piece)
{
    for (int i = 0; i < PIECE_MAT_SIZE; i++)
    {
        short int terrainCoord = (*(Piece **)piece)->ToTerrainCoord(0, i);

        for (int j = 0; j < PIECE_MAT_SIZE; j++)
        {
            if ((**(Piece **)piece)(i, j) == 1)
            {
                terrainCoord = (*(Piece **)piece)->ToTerrainCoord(j, i);
                if (terrainCoord < 0) // Si les coordonnées pointent en dehors du terrain
                    continue;

                matrix[terrainCoord] = (*piece)->GetColorChar();
            }
        }

        // Si la ligne est complète
        if (CheckLine(terrainCoord))
            RemoveLine(terrainCoord);
    }

    // Générer une nouvelle pièce
    delete *piece;
    *piece = SpawnPiece();

    Render_Terrain(*piece);
}