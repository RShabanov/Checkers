#pragma once
#include "Color.h"
#include "Piece.h"

#include <vector>

struct Pieces {
	using PieceSet = std::vector<std::shared_ptr<Piece>>;

	PieceSet white, black;
	unsigned int whiteKingN, blackKingN;

	explicit Pieces();
	explicit Pieces(const Pieces&);
	~Pieces() = default;
};

