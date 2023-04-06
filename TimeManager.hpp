#ifndef PIECES_MANAGER_HPP
#define PIECES_MANAGER_HPP

#include "Random.hpp"
#include "TetrisElements_Graphic-Piece.hpp"
#include "TetrisElements_Graphic-Terrain.hpp"

#include <cassert>
#include <deque>

#define SPAWN_POS (TERR_NBR_COL / 2) - (PIECE_MAT_SIZE / 2)
#define MAX_PREDICTION 3 // Nombre maximal de pièces dans le futur

// Forward declaration pour éviter les erreurs 'TerrainGraphic has not been declared'
class TerrainGraphic;

class FuturPiecesManager
{
  private:
	// Mémorisation des pièces placées dans le futur
	// Contient en placedPieces[t] l'ensemble des pièces placées entre la pièce t et la pièce t+1
	std::list<PieceGraphic *> placedPieces[MAX_PREDICTION];

	// Indice de l'élément correspondant au temps présent
	int idBeginning;

  public:
	FuturPiecesManager();

	// Efface toutes les pièces de la liste en position futureOffset
	void ClearAt(int futureOffset);

	// Réinitialise les listes
	void Reset();

	// Retourne la liste des PieceGraphic qui ont été placée entre futureOffset et futurOffset + 1 pas dans le futur par rapport au temps actuel
	std::list<PieceGraphic *> operator()(const int futureOffset) const;

	// Avance d'un pas dans le temps (Attention, on perd les pièces futures en position actuelle)
	void MoveOneUnit();

	// Ajoute la pièce au tableau des pièces à placer dans le futur
	void AddToFuturPlaced(const int futureOffset, PieceGraphic *piece);
};

class TimeManager
{
  private:
	// On utilise deux double queue pour pouvoir se déplacer dans le temps
	// La taille de previousPieceQueue correspond à la position actuelle dans le temps
	std::deque<PieceGraphic *> piecesQueue, previousPiecesQueue;

	// Gestion des pièces placées dans le futur
	FuturPiecesManager placedPieces;

	// Génère une PieceGraphic avec les bons paramètres pour être spawn, puis en retourne une référence
	PieceGraphic *GenerateOnePiece();

  public:
	~TimeManager();

	// Rempli la queue future avec le nombre maximal de pièces
	void GeneratePieces();

	// Retourne la prochaine pièce et en génère une autre dans la queue
	PieceGraphic *GetNextPiece();

	// Retourne la première pièce de la queue sans la retirer, pour affichage
	const PieceGraphic *SeeNextPiece() const;

	// Retourne la dernière pièce de la previousQueue sans la retirer, pour affichage
	const PieceGraphic *SeePreviousPiece() const;

	// Change la pièce actuelle par la pièce suivante et retient la pèce actuelle
	bool MoveInTime(PieceGraphic **currentPiece, TerrainGraphic *terrainGraph);

	// Change la pièce actuelle par la pièce précédente et retient la pèce actuelle
	void BackInTime(PieceGraphic **currentPiece, TerrainGraphic *terrainGraph);

	// Réinitialise les queues et les listes
	void Reset();

	// Retourne vrai si la pièce actuelle est dans le futur
	bool IsInFuture();

	// Ajoute la pièce à la liste des pièces placées au temps actuelle
	void AddFuturPlaced(PieceGraphic *piece);

	// Avance d'un pas dans le temps
	bool MoveOneTimeUnit(TerrainGraphic *terrainGraph);

	// Retourne la position actuelle dans le temps (0 est le présent)
	int GetTimePosition();
};

#endif