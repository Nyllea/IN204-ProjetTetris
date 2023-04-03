#include "TetrisElements.hpp"

Piece &Piece::operator=(const Piece &source)
{
	m_type = source.m_type;
	matrix = source.matrix;

	posX = source.posX;
	posY = source.posY;

	return *this;
}

Piece::Piece(const Type type) : m_type(type), posX(0), posY(0)
{
	switch (type)
	{
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

// Permet d'accéder à la case (x,y) de la matrice de la pièce
bool Piece::operator()(const unsigned char x, const unsigned char y) const
{
	if (x < PIECE_MAT_SIZE && y < PIECE_MAT_SIZE)
		return matrix[PIECE_MAT_SIZE * x + y];
	else
	{
		std::cout << "Error - Piece matrix index out of bounds" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Drop bits outside the range [R, L) == [R, L - 1]
template <std::size_t N>
std::bitset<N> project_range(std::bitset<N> b, size_t R, size_t L)
{
	if (R > L && L > N)
	{
		printf("Invalid bitrange\n");
		exit(EXIT_FAILURE);
	}
	b >>= R;		   // drop R rightmost bits
	b <<= (N - L + R); // drop L-1 leftmost bits
	b >>= (N - L);	   // shift back into place
	return b;
}

// Set the Y position for the piece to touch the upper limit of the terrain
void Piece::SetMaxHeight()
{
	while (project_range<PIECE_MAT_SIZE * PIECE_MAT_SIZE>(matrix, -posY * PIECE_MAT_SIZE, (-posY + 1) * PIECE_MAT_SIZE).none())
		posY--;
}

void Piece::SetXPos(const signed char xPos)
{
	posX = xPos;
}

void Piece::RotateLeft()
{
	if (m_type == Square)
		return;

	std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> temp(matrix);

	for (unsigned char i = 0; i < PIECE_MAT_SIZE; i++)
	{
		for (unsigned char j = 0; j < PIECE_MAT_SIZE; j++)
		{
			matrix[PIECE_MAT_SIZE * i + j] = temp[PIECE_MAT_SIZE * (4 - j) + i];
		}
	}
}

void Piece::RotateRight()
{
	if (m_type == Square)
		return;

	std::bitset<PIECE_MAT_SIZE * PIECE_MAT_SIZE> temp(matrix);

	for (unsigned char i = 0; i < PIECE_MAT_SIZE; i++)
	{
		for (unsigned char j = 0; j < PIECE_MAT_SIZE; j++)
		{
			matrix[PIECE_MAT_SIZE * i + j] = temp[PIECE_MAT_SIZE * j + (4 - i)];
		}
	}
}

// Vérifie si la pièce occupe la case (x,y) du terrain (en fonction de sa position et de sa matrice)
bool Piece::IsAt(const unsigned char x, const unsigned char y) const
{
	if (y < posY || y >= posY + PIECE_MAT_SIZE || x < posX || x >= posX + PIECE_MAT_SIZE)
		return false;

	return matrix[(y - posY) * PIECE_MAT_SIZE + (x - posX)] == 1;
	// Idée: utiliser des opérations sur les bits directement
	// return (matrix & (1<<(PIECE_MAT_SIZE*PIECE_MAT_SIZE - (y - posY) * PIECE_MAT_SIZE + (x - posX)))) == 0;
}

// Déplace la pièce de dirX selon X et de dirY selon Y
void Piece::Move(const signed char dirX, const signed char dirY)
{
	posX += dirX;
	posY += dirY;
}

// Retourne la position de la case (x,y) de la matrice de la pièce dans les coordonnées de la matrice du terrain (négatif si hors du terrain)
short int Piece::ToTerrainCoord(const unsigned char x, const unsigned char y) const
{
	return posY + y >= 0 && posX + x >= 0 && posY + y < TERR_NBR_LINES && posX + x < TERR_NBR_COL ? (posY + y) * TERR_NBR_COL + posX + x : -1;
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
	for (int i = 0; i < PIECE_MAT_SIZE; i++)
	{
		for (int j = 0; j < PIECE_MAT_SIZE; j++)
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