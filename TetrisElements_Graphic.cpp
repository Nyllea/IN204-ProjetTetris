#include "TetrisElements_Graphic.hpp"

// Permet d'acceder à la couleur (getter pour m_color)
char PieceGraphic::GetColorChar() const { return (char)m_color; }

// Convertit le code couleur(char) en une couleur GdkRGBA
Gdk::RGBA GridGraphic::CharToColor(const char colorVal) const
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

// Setter pour la grille graphique
void GridGraphic::SetGrid(Gtk::Grid *const _grid) { grid = _grid; }

// Actualisation graphique de la grille
void GridGraphic::RenderGrid(const PieceGraphic *const piece, const int lineNbr, const int colNbr, const char *matrix)
{
	Gtk::Widget *block;
	Gdk::RGBA color;

	for (unsigned char i = 0; i < lineNbr; i++)
	{
		for (unsigned char j = 0; j < colNbr; j++)
		{
			block = grid->get_child_at(j, i);

			if (piece != NULL && ((Piece *)piece)->IsAt(j, i))
				color = CharToColor(piece->GetColorChar());
			else if (matrix != NULL)
				color = CharToColor(matrix[TERR_NBR_COL * i + j]);
			else
				color = Gdk::RGBA(emptyColor);

			block->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
		}
	}
}

// Remplissage de la grille graphique avec des blocs vides
void GridGraphic::FillGrid(const int windowHeight, const int windowWidth, const int lineNbr, const int colNbr)
{
	gint blockSize = std::min(windowWidth, windowHeight) / std::max(TERR_NBR_LINES, TERR_NBR_COL);

	for (int i = 0; i < lineNbr; i++)
	{
		for (int j = 0; j < colNbr; j++)
		{
			Gtk::Label *temp = Gtk::make_managed<Gtk::Label>();
			temp->override_background_color(Gdk::RGBA(emptyColor), Gtk::STATE_FLAG_NORMAL);
			temp->set_size_request(blockSize, blockSize);
			grid->attach(*temp, j, i, 1, 1);
		}
	}
}

// Supprime l'ancienne pièce et en génère une nouvelle
void TerrainGraphic::SpawnRandomPiece(PieceGraphic **const piece)
{
	if (*piece != NULL)
		delete *piece;

	*piece = CreateRandomPiece();
}

// Réinitialise le terrain avec une nouvelle pièce
void TerrainGraphic::ResetTerrain(PieceGraphic **const piece)
{
	ResetMatrix();
	SpawnRandomPiece(piece);
	RenderGrid(*piece);
}

// Instancie une piece sur le terrain et en retourne une reference
PieceGraphic *TerrainGraphic::CreateRandomPiece() const
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

	SpawnRandomPiece(piece);
	RenderGrid(*piece);
}