#include "Man.h"

Man::Man(const Position& position, Color color)
	: Figure(position, color) {}

std::vector<Position> Man::possibleMoves(const Board&) const {
	return std::vector<Position>();
}

bool Man::isQueen() const {	return false; }
