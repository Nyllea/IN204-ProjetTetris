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

// Actualisation graphique de la grille (affiche une grille vide si piece=NULL, sinon affiche la matrice de la pièce si matrix=NULL)
void GridGraphic::RenderGrid(const PieceGraphic *const piece, const int lineNbr, const int colNbr, const char *matrix)
{
	Gtk::Widget *block;
	Gdk::RGBA color = Gdk::RGBA(emptyColor);

	for (unsigned char i = 0; i < lineNbr; i++)
	{
		for (unsigned char j = 0; j < colNbr; j++)
		{
			// On récupère le bloc en position (j, i)
			block = grid->get_child_at(j, i);

			// Si pas de pièce fournie, on affiche tout de la couleur emptyColor
			if (piece != NULL)
			{
				// Si la position (i,j) est occupée par la pièce, on prend la couleur de la pièce
				if (((Piece *)piece)->IsAt(j, i, matrix != NULL))
					color = CharToColor(piece->GetColorChar());
				else if (matrix != NULL)
					color = CharToColor(matrix[TERR_NBR_COL * i + j]);
				else // Si matrix=NULL et que la pièce n'occupe pas la position (j, i)
					color = Gdk::RGBA(emptyColor);
			}

			block->override_background_color(color, Gtk::STATE_FLAG_NORMAL);
		}
	}
}

// Setter pour la grille graphique
void GridGraphic::SetGrid(Gtk::Grid *const _grid) { grid = _grid; }

// Réinitialise le terrain et le score et génère une nouvelle pièce
void TerrainGraphic::ResetTerrain(PieceGraphic **const piece, TimeManager &timeManager)
{
	ResetMatrix();
	ResetScore();
	timeManager.GeneratePieces();
	*piece = timeManager.GetNextPiece();
	RenderGrid(*piece);
}

// Ajoute la piece à la matrice du terrain
// Si c'est une pièce dans le présent, enlève les lignes complètes et génère une nouvelle piece
// Si c'est une pièce dans le futur, l'ajoute aux pièces placées et génère une nouvelle pièce
// Retourne false s'il y a eu une collision
bool TerrainGraphic::ImprintPiece(PieceGraphic **const piece, TimeManager &timeManager)
{
	int completedLines = 0;
	bool noCollision = true;

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

		// Si la ligne est complète et que ce n'est pas le futur
		if (!timeManager.IsInFuture() && CheckLine(terrainCoord))
		{
			RemoveLine(terrainCoord);
			completedLines++;
		}
	}

	// Si c'est une pièce dans le futur
	if (timeManager.IsInFuture())
	{
		// On l'ajoute aux pièces placées dans le futures
		timeManager.AddFuturPlaced(*piece);
	}
	else
	{
		// On supprime la pièce, vu qu'elle est maintenant placée de manière permanente
		delete *piece;

		// On avance d'un pas de temps, étant donnée que l'on vient de placer la pièce au temps actuel
		noCollision = timeManager.MoveOneTimeUnit(this);

		UpdateScore(completedLines);
	}

	// On récupère une nouvelle pièce
	*piece = timeManager.GetNextPiece();

	RenderGrid(*piece);

	return noCollision;
}

// Ajoute la piece à la matrice du terrain et retourne false s'il y a une collision
// Si removeLine, supprime les lignes complétées par les pièces et actualise le score
bool TerrainGraphic::ImprintFuturePieces(const std::list<PieceGraphic *> &pieces, const bool removeLines)
{
	int completedLines = 0;
	bool noCollision = true;

	// Pour chaque pièce
	for (PieceGraphic *const &piece : pieces)
	{
		for (int i = 0; i < PIECE_MAT_SIZE; i++)
		{
			short int terrainCoord = ((Piece *)piece)->ToTerrainCoord(0, i);

			for (int j = 0; j < PIECE_MAT_SIZE; j++)
			{
				if ((*(Piece *)piece)(i, j) == 1)
				{
					terrainCoord = ((Piece *)piece)->ToTerrainCoord(j, i);
					if (terrainCoord < 0) // Si les coordonnées pointent en dehors du terrain
						continue;

					// Si la case était vide, on met la bonne couleur
					if (matrix[terrainCoord] != 0)
						noCollision = false;

					matrix[terrainCoord] = piece->GetColorChar();
				}
			}

			// Si la ligne est complète et que on supprime les lignes
			if (removeLines && CheckLine(terrainCoord))
			{
				RemoveLine(terrainCoord);
				completedLines++;
			}
		}
	}

	UpdateScore(completedLines);

	return noCollision;
}

// Enlève les pieces de la matrice du terrain et affiche le terrain
void TerrainGraphic::DeprintFuturePieces(const std::list<PieceGraphic *> &pieces)
{
	// Pour chaque pièce
	for (PieceGraphic *const &piece : pieces)
	{
		for (int i = 0; i < PIECE_MAT_SIZE; i++)
		{
			for (int j = 0; j < PIECE_MAT_SIZE; j++)
			{
				if ((*(Piece *)piece)(i, j) == 1)
				{
					short int terrainCoord = ((Piece *)piece)->ToTerrainCoord(j, i);
					if (terrainCoord < 0) // Si les coordonnées pointent en dehors du terrain
						continue;

					// La case devient vide (grise)
					matrix[terrainCoord] = 0;
				}
			}
		}
	}
}