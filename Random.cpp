#include "Random.hpp"

static std::random_device rd;
static std::mt19937 gen(rd());

PieceGraphic::Color GetRandomColor() {
    std::uniform_int_distribution<size_t> dis(1, ALL_COLORS.size() - 1);
    return ALL_COLORS[dis(gen)];
}

Piece::Type GetRandomType() {
    std::uniform_int_distribution<size_t> dis(0, ALL_TYPES.size() - 1);
    return ALL_TYPES[dis(gen)];
}