#pragma once
#include "Color.h"
#include "Position.h"

#include <vector>
#include <array>

class Figure;

using Board = std::array<std::array<Figure*, BOARD_SIZE>, BOARD_SIZE>;

class Figure {
	Color color;
	Position position;

public:
	explicit Figure(const Position&, Color);
	virtual ~Figure() = default;

	virtual std::vector<Position> possibleMoves(const Board&) const = 0;
	virtual bool isQueen() const = 0;
	Color getColor() const;

	char getX() const;
	char getY() const;
};

