#include "TimeManager.hpp"

FuturPiecesManager::FuturPiecesManager() : idBeginning(0)
{
	for (int i = 0; i < MAX_PREDICTION; i++)
		placedPieces[i].clear();
}

void FuturPiecesManager::ClearAt(int futureOffset)
{
	int id = (idBeginning + futureOffset) % MAX_PREDICTION;

	for (; !placedPieces[id].empty(); placedPieces[id].pop_front())
		delete placedPieces[id].front();
}

// Reset les listes
void FuturPiecesManager::Reset()
{
	for (int i = 0; i < MAX_PREDICTION; i++)
		for (; !placedPieces[i].empty(); placedPieces[i].pop_front())
			delete placedPieces[i].front();
}

// Retourne la liste des PieceGraphic qui ont été placée entre futureOffset et futurOffset + 1 pas dans le futur par rapport au temps actuel
std::list<PieceGraphic *> FuturPiecesManager::operator()(const int futureOffset) const
{
	assert(futureOffset < MAX_PREDICTION);

	return placedPieces[(idBeginning + futureOffset) % MAX_PREDICTION];
}

// Avance d'un pas dans le temps (Attention, on perd les pièces futures en position actuelle)
void FuturPiecesManager::MoveOneUnit()
{
	placedPieces[idBeginning % MAX_PREDICTION].clear();

	idBeginning++;
}

// Ajoute la pièce au tableau des pièces à placer dans le futur
void FuturPiecesManager::AddToFuturPlaced(const int futureOffset, PieceGraphic *piece)
{
	assert(futureOffset < MAX_PREDICTION);

	placedPieces[(idBeginning + futureOffset) % MAX_PREDICTION].push_front(piece);
}

TimeManager::~TimeManager()
{
	for (; !piecesQueue.empty(); piecesQueue.pop_front())
		delete piecesQueue.front();

	for (; !previousPiecesQueue.empty(); previousPiecesQueue.pop_front())
		delete previousPiecesQueue.front();
}

// Génère une PieceGraphic avec les bons paramètres pour être spawn, puis en retourne une référence
PieceGraphic *TimeManager::GenerateOnePiece()
{
	PieceGraphic *piece = new PieceGraphic(GetRandomType(), GetRandomColor());

	((Piece *)piece)->SetMaxHeight();
	((Piece *)piece)->SetXPos(SPAWN_POS);

	return piece;
}

// Rempli la queue avec le nombre maximal de pièces
void TimeManager::GeneratePieces()
{
	while (piecesQueue.size() < MAX_PREDICTION)
		piecesQueue.push_back(GenerateOnePiece());
}

// Retourne la prochaine pièce et en génère une autre dans la queue
PieceGraphic *TimeManager::GetNextPiece()
{
	// On ajoute une nouvelle pièce à la queue
	piecesQueue.push_back(GenerateOnePiece());

	PieceGraphic *piece = piecesQueue.front();

	// On retire le premier élément de la queue
	piecesQueue.pop_front();

	return piece;
}

// Retourne la première pièce de la queue sans la retirer, pour affichage
const PieceGraphic *TimeManager::SeeNextPiece() const
{
	return !piecesQueue.empty() ? piecesQueue.front() : NULL;
}

// Retourne la dernière pièce de la previousQueue sans la retirer, pour affichage
const PieceGraphic *TimeManager::SeePreviousPiece() const
{
	return !previousPiecesQueue.empty() ? previousPiecesQueue.back() : NULL;
}

// Change la pièce actuelle par la pièce suivante et retient la pèce actuelle
// Retourne false s'il y a eu une collision
bool TimeManager::MoveInTime(PieceGraphic **currentPiece, TerrainGraphic *terrainGraph)
{
	// On ne se déplace dans le temps que si il reste au moins un élément dans la file piecesQueue
	if (previousPiecesQueue.size() < MAX_PREDICTION - 1)
	{
		previousPiecesQueue.push_back(*currentPiece);
		*currentPiece = piecesQueue.front();
		piecesQueue.pop_front();

		// S'il y a eu une collision
		return terrainGraph->ImprintFuturePieces(placedPieces(previousPiecesQueue.size()), false);
	}
	else
		return true;
}

// Change la pièce actuelle par la pièce précédente et retient la pèce actuelle
void TimeManager::BackInTime(PieceGraphic **currentPiece, TerrainGraphic *terrainGraph)
{
	// On ne se déplace dans le temps que si il reste au moins un élément dans la file previousPiecesQueue
	if (!previousPiecesQueue.empty())
	{
		// S'il y a eu une collision
		terrainGraph->DeprintFuturePieces(placedPieces(previousPiecesQueue.size()));

		piecesQueue.push_front(*currentPiece);
		*currentPiece = previousPiecesQueue.back();
		previousPiecesQueue.pop_back();
	}
}

// Reset the queues and lists
void TimeManager::Reset()
{
	for (; !piecesQueue.empty(); piecesQueue.pop_front())
		delete piecesQueue.front();

	for (; !previousPiecesQueue.empty(); previousPiecesQueue.pop_front())
		delete previousPiecesQueue.front();

	placedPieces.Reset();

	GeneratePieces();
}

// Is the current piece in the future
bool TimeManager::IsInFuture() { return !previousPiecesQueue.empty(); }

// Ajoute la pièce à la liste des pièces placées au temps actuelle
void TimeManager::AddFuturPlaced(PieceGraphic *piece) { placedPieces.AddToFuturPlaced(previousPiecesQueue.size(), piece); }

// Avance d'un pas dans le temps
bool TimeManager::MoveOneTimeUnit(TerrainGraphic *terrainGraph)
{
	// On décale l'origine du temps de 1
	placedPieces.MoveOneUnit();

	// On ajoute toutes les pièces qui avait été placées 1 pas dans le future au terrain
	bool noCollision = terrainGraph->ImprintFuturePieces(placedPieces(0), true);

	// On supprime toutes ces pièces
	placedPieces.ClearAt(0);

	return noCollision;
}

// Retourne la position actuelle dans le temps (0 est le présent)
int TimeManager::GetTimePosition() { return previousPiecesQueue.size(); }