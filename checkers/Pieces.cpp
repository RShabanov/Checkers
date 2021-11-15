#include "Pieces.h"

Pieces::Pieces()
	: white(), black(), whiteKingN(0), blackKingN(0) {}

Pieces::Pieces(const Pieces& pieces)
	: white(pieces.white),
	black(pieces.black),
	blackKingN(pieces.blackKingN),
	whiteKingN(pieces.whiteKingN) {}