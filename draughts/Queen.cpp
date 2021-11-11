#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Moves Queen::possibleMoves(const Board& board) const {
	Moves moves;

	

	return moves;
}

bool Queen::isQueen() const { return true; }
