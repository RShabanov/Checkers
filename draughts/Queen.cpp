#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

std::vector<Position> Queen::possibleMoves(const Board&) const
{
	return std::vector<Position>();
}

bool Queen::isQueen() const { return true; }
