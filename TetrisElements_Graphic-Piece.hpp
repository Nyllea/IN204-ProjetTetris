#ifndef TETRISELEMENTS_GRAPHIC_PIECE_HPP
#define TETRISELEMENTS_GRAPHIC_PIECE_HPP

#include "TetrisElements-Piece.hpp"

#include <cassert>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>

class PieceGraphic : Piece
{
  public:
	enum Color
	{
		Grey = 0,
		Red = 1,
		Green = 2,
		Blue = 3
	};

  private:
	Color m_color;

  public:
	// Constructeurs
	PieceGraphic(const Piece *const piece, const Color color) : Piece(*piece), m_color(color){};
	PieceGraphic(const Type type, const Color color) : Piece(type), m_color(color){};

	PieceGraphic(const PieceGraphic &source) : Piece((Piece)source), m_color(source.m_color){};

	// Permet d'acceder à la couleur de la pièce (getter pour m_color)
	char GetColorChar() const;
};

#endif