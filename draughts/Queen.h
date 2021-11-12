#pragma once
#include "Figure.h"
#include "Man.h"

#include <algorithm>

class Queen : public Figure {
	Moves getPossibleAttacks(const Board& board, const Position& pos) const;

public:
	explicit Queen(const Position&, Color);
	explicit Queen(Man&&);
	virtual ~Queen() = default;

	Moves possibleMoves(const Board&) const override;
	bool isQueen() const override;
};

