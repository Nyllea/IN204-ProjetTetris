#include "TetrisElements_Graphic-Terrain.hpp"

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
	Gdk::RGBA color = Gdk::RGBA(emptyColor);

	

	for (unsigned char i = 0; i < lineNbr; i++)
	{
		for (unsigned char j = 0; j < colNbr; j++)
		{
			block = grid->get_child_at(j, i);

			// Si pas de pièce fournie, on affiche tout de la couleur emptyColor
			if (piece != NULL)
			{
				// Si la position (i,j) est occupée par la pièce
				if (((Piece *)piece)->IsAt(j, i, matrix != NULL))
					color = CharToColor(piece->GetColorChar());
				else if (matrix != NULL)
					color = CharToColor(matrix[TERR_NBR_COL * i + j]);
				else
					color = Gdk::RGBA(emptyColor);
			}

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
void TerrainGraphic::SpawnRandomPiece(PieceGraphic **const piece, PiecesManager &piecesManager)
{
	if (*piece != NULL)
		delete *piece;

	*piece = piecesManager.GetNextPiece();
}

// Réinitialise le terrain avec une nouvelle pièce
void TerrainGraphic::ResetTerrain(PieceGraphic **const piece, PiecesManager &piecesManager)
{
	ResetMatrix();
	ResetScore();
	piecesManager.GeneratePieces();
	*piece = piecesManager.GetNextPiece();
	RenderGrid(*piece);
}

// Ajoute la piece à la matrice du terrain, enlève les lignes complètes et génère une nouvelle piece
void TerrainGraphic::ImprintPiece(PieceGraphic **const piece, PiecesManager &piecesManager)
{	
	int cplt_lines=0;
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
		{
			RemoveLine(terrainCoord);
			cplt_lines+=1;
		}
	}
	updatescore(cplt_lines);
	SpawnRandomPiece(piece, piecesManager);
	RenderGrid(*piece);


}