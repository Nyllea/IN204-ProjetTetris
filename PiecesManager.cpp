#include "PiecesManager.hpp"

PiecesManager::~PiecesManager()
{
	for (; !piecesQueue.empty(); piecesQueue.pop_front())
		delete piecesQueue.front();

	for (; !previousPiecesQueue.empty(); previousPiecesQueue.pop_front())
		delete previousPiecesQueue.front();
}

// Génère une PieceGraphic avec les bons paramètres pour être spawn, puis en retourne une référence
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

// Retourne la première pièce de la queue sans la retirer, pour affichage
const PieceGraphic *PiecesManager::SeeNextPiece() const
{
	return !piecesQueue.empty() ? piecesQueue.front() : NULL;
}

// Retourne la dernière pièce de la previousQueue sans la retirer, pour affichage
const PieceGraphic *PiecesManager::SeePreviousPiece() const
{
	return !previousPiecesQueue.empty() ? previousPiecesQueue.back() : NULL;
}

// Change la pièce actuelle par la pièce suivante et retient la pèce actuelle
void PiecesManager::MoveInTime(PieceGraphic **currentPiece)
{
	// On ne se déplace dans le temps que si il reste au moins un élément dans la file piecesQueue
	if (previousPiecesQueue.size() < MAX_PREDICTION - 1)
	{
		previousPiecesQueue.push_back(*currentPiece);
		*currentPiece = piecesQueue.front();
		piecesQueue.pop_front();
	}
}

// Change la pièce actuelle par la pièce précédente et retient la pèce actuelle
void PiecesManager::BackInTime(PieceGraphic **currentPiece)
{
	// On ne se déplace dans le temps que si il reste au moins un élément dans la file previousPiecesQueue
	if (!previousPiecesQueue.empty())
	{
		piecesQueue.push_front(*currentPiece);
		*currentPiece = previousPiecesQueue.back();
		previousPiecesQueue.pop_back();
	}
}