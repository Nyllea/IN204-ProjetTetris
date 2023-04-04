#include "PiecesManager.hpp"

PiecesManager::~PiecesManager()
{
	for (; !piecesQueue.empty(); piecesQueue.pop_front())
		delete piecesQueue.front();
}

PieceGraphic *PiecesManager::GenerateOnePiece()
{
	PieceGraphic *piece = new PieceGraphic(GetRandomType(), GetRandomColor());

	((Piece *)piece)->SetMaxHeight();
	((Piece *)piece)->SetXPos(SPAWN_POS);

	return piece;
}

// Rempli la queue avec le nombre maximal de pièces
void PiecesManager::GeneratePieces()
{
	while (piecesQueue.size() < MAX_PREDICTION)
		piecesQueue.push_back(GenerateOnePiece());
}

// Retourne la prochaine pièce et en génère une autre dans la queue
PieceGraphic *PiecesManager::GetNextPiece()
{
	// On ajoute une nouvelle pièce à la queue
	piecesQueue.push_back(GenerateOnePiece());

	PieceGraphic *piece = piecesQueue.front();

	// On retire le premier élément de la queue
	piecesQueue.pop_front();

	return piece;
}

const PieceGraphic *PiecesManager::SeeNextPiece() const
{
	return piecesQueue.front();
}