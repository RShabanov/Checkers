#pragma once
#include "Color.h"
#include "Position.h"

class GameBoard;

class Piece {
protected:
	Color color;
	Position position;

public:
	explicit Piece(const Position&, Color);
	virtual ~Piece() = default;

	virtual Moves possibleMoves(const GameBoard&, const Position& from, bool onlyAttack = false) const = 0;
	virtual bool isKing() const = 0;
	//virtual void move()

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

// so inherited class can use
GameBoard&& copyBoard(const GameBoard&);
void movePiece(GameBoard*, const Position& from, const Position& to);
