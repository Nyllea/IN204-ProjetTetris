#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#include "./Affichage-Gtk/TetrisElements_Graphic-Piece.hpp"

static std::vector<PieceGraphic::Color> ALL_COLORS({PieceGraphic::Color::Grey, PieceGraphic::Color::Red, PieceGraphic::Color::Green, PieceGraphic::Color::Blue});
static std::vector<Piece::Type> ALL_TYPES({Piece::Type::Square, Piece::Type::I, Piece::Type::L, Piece::Type::LMirrored, Piece::Type::N, Piece::Type::NMirrored, Piece::Type::T});

PieceGraphic::Color GetRandomColor();
Piece::Type GetRandomType();

#endif