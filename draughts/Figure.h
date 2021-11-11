#pragma once
#include "Color.h"
#include "Position.h"

#include <vector>
#include <array>

class Figure;

using Board = std::array<std::array<Figure*, BOARD_SIZE>, BOARD_SIZE>;
using Moves = std::vector<std::vector<Position>>;

class Figure {
protected:
	Color color;
	Position position;

public:
	explicit Figure(const Position&, Color);
	virtual ~Figure() = default;

	virtual Moves possibleMoves(const Board&) const = 0;
	virtual bool isQueen() const = 0;
	Color getColor() const;

	char getX() const;
	char getY() const;
};

