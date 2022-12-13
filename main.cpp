#include <iostream>
#include <gtk/gtk.h>

#include "TetrisElements.hpp"

void PrintPiece(Piece piece) {
	for (int i = 0; i < PIECE_MAT_SIZE; i++) {
    	for (int j = 0; j < PIECE_MAT_SIZE; j++) {
            std::cout << piece(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

void PrintTerrain(Terrain terrain) {
	for (int i = 0; i < TERR_NBR_LINES; i++) {
    	for (int j = 0; j < TERR_NBR_COL; j++) {
            std::cout << (int)terrain(i,j);
        }
		std::cout << std::endl;
    }
	std::cout << std::endl;
}

int main() {
	std::cout << "Tet" << std::endl;

	Piece squa(Piece::Type::N);

	PrintPiece(squa);

	squa.RotateLeft();

	PrintPiece(squa);

	squa.RotateRight();

	PrintPiece(squa);

	Terrain terrain;

	PrintTerrain(terrain);

	return 0;
}
