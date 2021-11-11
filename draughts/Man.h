#pragma once
#include "Figure.h"

class Man final : public Figure {
public:
	explicit Man(const Position&, Color);
	virtual ~Man() = default;

	std::vector<Position> possibleMoves(const Board&) const override;
	bool isQueen() const override;
};

