#ifndef PIECES_MANAGER_HPP
#define PIECES_MANAGER_HPP

#include "Random.hpp"
#include "TetrisElements_Graphic-Piece.hpp"

#include <deque>

#define SPAWN_POS (TERR_NBR_COL / 2) - (PIECE_MAT_SIZE / 2)
#define MAX_PREDICTION 3 // Nombre maximal de pièces dans le futur

class PiecesManager
{
  private:
	// On utilise une double queue pour pouvoir se déplacer dans le temps
	std::deque<PieceGraphic *> piecesQueue, previousPiecesQueue;

	// Génère une PieceGraphic avec les bons paramètres pour être spawn, puis en retourne une référence
	PieceGraphic *GenerateOnePiece();

  public:
	~PiecesManager();

	// Rempli la queue avec le nombre maximal de pièces
	void GeneratePieces();

	// Retourne la prochaine pièce et en génère une autre dans la queue
	PieceGraphic *GetNextPiece();

	// Retourne la première pièce de la queue sans la retirer, pour affichage
	const PieceGraphic *SeeNextPiece() const;

	// Retourne la dernière pièce de la previousQueue sans la retirer, pour affichage
	const PieceGraphic *SeePreviousPiece() const;

	// Change la pièce actuelle par la pièce suivante et retient la pèce actuelle
	void MoveInTime(PieceGraphic **currentPiece);

	// Change la pièce actuelle par la pièce précédente et retient la pèce actuelle
	void BackInTime(PieceGraphic **currentPiece);
};

#endif