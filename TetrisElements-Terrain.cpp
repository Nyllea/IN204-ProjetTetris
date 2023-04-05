#include "TetrisElements-Terrain.hpp"

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

// Permet d'accéder à la valeur(char) du bloc en position (x,y) du terrain
char Terrain::operator()(const unsigned char x, const unsigned char y) const
{
	if (x < TERR_NBR_LINES && y < TERR_NBR_COL)
		return matrix[TERR_NBR_COL * x + y];
	else
	{
		std::cout << "Error - Terrain matrix index out of bounds" << std::endl;
		exit(EXIT_FAILURE);
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

// Vérifie si la ligne contenant le bloc en terrainCoord est complète
bool Terrain::CheckLine(const short int terrainCoord)
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

// Réinitialise le terrain
void Terrain::ResetMatrix()
{
	for (int i = 0; i < TERR_NBR_LINES * TERR_NBR_COL; i++)
		matrix[i] = 0;
}


void Terrain::updatescore(int cplt_lines)
{   
	
	int level=1+cleared_lines/10;
	
    switch (cplt_lines)
    {
	case 1:
		score+=100*level;
		break;
    case 2:
        score+=300*level;
        break;
    case 3:
        score+=500*level;
        break;
    case 4:
        score+=800*level;
        break;
    }
	
	// std::string temp_str=std::to_string(score); //converting number to a string
	// std::cout << temp_str << "\n";
	cleared_lines+=cplt_lines;
    
}


int Terrain::get_cleared_lines()
{
	return cleared_lines;
}

int Terrain::get_score()
{
	return score;
}



void Terrain::ResetScore()
{
	score=0;
	cleared_lines=0;
}

