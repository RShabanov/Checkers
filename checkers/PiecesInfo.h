#pragma once
#include "Color.h"
#include "Piece.h"

#include <vector>

struct PiecesInfo {
	char whiteKingN, blackKingN, whiteN, blackN;

	explicit PiecesInfo();
	PiecesInfo(const PiecesInfo&);
	~PiecesInfo() = default;
};

