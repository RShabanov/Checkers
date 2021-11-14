#pragma once
#include "Color.h"
#include "Position.h"

class GameBoard; // solves class usage problem

class Figure {
protected:
	Color color;
	Position position;

public:
	explicit Figure(const Position&, Color);
	virtual ~Figure() = default;

	virtual Moves possibleMoves(const GameBoard&, bool onlyAttack = false) const = 0;
	virtual bool isQueen() const = 0;
	bool isWhite() const;
	bool isBlack() const;
	Color getColor() const;

	const Position& getPosition() const;
	char getX() const;
	char getY() const;

	void setPosition(char, char);
	void setPosition(const Position&);
	void setX(char);
	void setY(char);
};

