#pragma once
#include "Figure.h"

class Queen : public Figure {
public:
	explicit Queen(const Position&, Color);
	virtual ~Queen() = default;

	Moves possibleMoves(const Board&) const override;
	bool isQueen() const override;
};

