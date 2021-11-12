#pragma once
#include "Figure.h"
#include "Board.h"
#include <algorithm>

class Man final : public Figure {
	void eatMove(const Board&, Position current, Position opponent, Moves*, size_t idx) const;

public:
	explicit Man(const Position&, Color);
	virtual ~Man() = default;

	Moves possibleMoves(const Board&) const override;
	bool isQueen() const override;
};

