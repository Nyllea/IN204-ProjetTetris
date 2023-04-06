#include "TetrisElements-Terrain.hpp"

// Réinitialise le terrain
void Terrain::ResetMatrix()
{
	for (int i = 0; i < TERR_NBR_LINES * TERR_NBR_COL; i++)
		matrix[i] = 0;
}

// Réinitialise le score
void Terrain::ResetScore()
{
	score = 0;
	clearedLines = 0;
}

// Vérifie si la ligne contenant le bloc en terrainCoord est complète
bool Terrain::CheckLine(const short int terrainCoord) const
{
	// Numéro de la ligne à vérifier
	short int line = terrainCoord / TERR_NBR_COL;

	for (int i = line * TERR_NBR_COL; i < (line + 1) * TERR_NBR_COL; i++)
	{
		if (matrix[i] == 0)
			return false;
	}

	return true;
}

// Retire la ligne contenant le bloc en terrainCoord
void Terrain::RemoveLine(const int terrainCoord)
{
	// Numéro de la ligne à retirer
	short int line = terrainCoord / TERR_NBR_COL;

	for (int i = line; i > 0; i--)
	{
		for (int j = 0; j < TERR_NBR_COL; j++)
		{
			matrix[i * TERR_NBR_COL + j] = matrix[(i - 1) * TERR_NBR_COL + j];
		}
	}

	for (int j = 0; j < TERR_NBR_COL; j++)
		matrix[j] = 0;
}

Terrain::Terrain()
{
	for (int i = 0; i < TERR_NBR_LINES; i++)
	{
		for (int j = 0; j < TERR_NBR_COL; j++)
		{
			matrix[TERR_NBR_COL * i + j] = 0;
		}
	}
}

// Retourne vrai si la pièce est hors du terrain ou touche un bloc, faux sinon
bool Terrain::CheckCollision(const Piece *const piece) const
{
	for (int i = 0; i < Piece::GetPieceMatSize(); i++)
	{
		for (int j = 0; j < Piece::GetPieceMatSize(); j++)
		{
			if ((*piece)(i, j) == 1)
			{
				short int terrCoord = piece->ToTerrainCoord(j, i);

				if (terrCoord < 0 || matrix[terrCoord] != 0)
					return true;
			}
		}
	}

	return false;
}

// Retourne le nombre de lignes complétées
int Terrain::GetClearedLines() const { return clearedLines; }

// Retourne le score
int Terrain::GetScore() const { return score; }

// Mets à jour le score et le nombre de lignes complétées
void Terrain::UpdateScore(int cplt_lines)
{

	int level = 1 + clearedLines / 10;

	switch (cplt_lines)
	{
		case 1:
			score += 100 * level;
			break;
		case 2:
			score += 300 * level;
			break;
		case 3:
			score += 500 * level;
			break;
		case 4:
			score += 800 * level;
			break;
	}

	clearedLines += cplt_lines;
}