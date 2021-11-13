#pragma once
#include "Figure.h"
#include "Board.h"

#include <algorithm>

class Queen : public Figure {
	Moves getPossibleAttacks(Board& board, const Position& pos) const;
	bool canAttackFrom(const Board&, const Position&) const;
public:
	explicit Queen(const Position&, Color);
	virtual ~Queen() = default;

	Moves possibleMoves(const Board&) const override;
	bool isQueen() const override;
};
