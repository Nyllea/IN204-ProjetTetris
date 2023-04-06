#include "TetrisElements-Piece.hpp"

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

// Retire les bits en dehors de [R, L) == [R, L - 1]
template <std::size_t N>
std::bitset<N> project_range(std::bitset<N> b, size_t R, size_t L)
{
	if (R > L && L > N)
	{
		printf("Invalid bitrange\n");
		exit(EXIT_FAILURE);
	}
	b >>= R;		   // Retire les R bits de droite
	b <<= (N - L + R); // Retire les L-1 bits de gauche
	b >>= (N - L);	   // Shift pour les remettre en place
	return b;
}

// Modifie la position Y de la pièce pour la placer en haut du terrain
void Piece::SetMaxHeight()
{
	while (project_range<PIECE_MAT_SIZE * PIECE_MAT_SIZE>(matrix, -posY * PIECE_MAT_SIZE, (-posY + 1) * PIECE_MAT_SIZE).none())
		posY--;
}

// Assigne la position X de la pièce
void Piece::SetXPos(const signed char xPos) { posX = xPos; }

// Tourne la pièce de 90 degrés vers la gauche (sens anti-horaire)
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

// Tourne la pièce de 90 degrés vers la droite (sens horaire)
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

// Si useTerrainPosition, vérifie si la pièce occupe la case (x,y) du terrain (en fonction de sa position et de sa matrice)
// Sinon vérifie si la case (x,y) de la matrice est occupée
bool Piece::IsAt(const unsigned char x, const unsigned char y, const bool useTerrainPosition) const
{
	int globalPosX = useTerrainPosition ? posX : 0;
	int globalPosY = useTerrainPosition ? posY : 0;

	if (y < globalPosY || y >= globalPosY + PIECE_MAT_SIZE || x < globalPosX || x >= globalPosX + PIECE_MAT_SIZE)
		return false;

	return matrix[(y - globalPosY) * PIECE_MAT_SIZE + (x - globalPosX)] == 1;

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
	return posY + y >= 0 && posX + x >= 0 && posY + y < Terrain::GetNbrLines() && posX + x < Terrain::GetNbrCol() ? (posY + y) * Terrain::GetNbrCol() + posX + x : -1;
}
