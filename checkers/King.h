#pragma once
#include "Piece.h"
#include "GameBoard.h"

class King : public Piece {
public:
	explicit King(const Position&, Color);
	virtual ~King() = default;

	Moves possibleMoves(const GameBoard&, const Position& from, bool onlyAttack = false) const;
	bool isKing() const;
};

