#pragma once
#include "Piece.h"
#include "GameBoard.h"

class Pawn : public Piece {
public:
	explicit Pawn(const Position&, Color);
	virtual ~Pawn() = default;

	Moves possibleMoves(const GameBoard&, const Position& from, bool onlyAttack = false) const;
	bool isKing() const;
};
