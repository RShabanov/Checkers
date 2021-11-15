#include "PiecesInfo.h"

PiecesInfo::PiecesInfo()
	: whiteN(0), blackN(0), whiteKingN(0), blackKingN(0) {}

PiecesInfo::PiecesInfo(const PiecesInfo& pieces)
	: whiteN(pieces.whiteN),
	blackN(pieces.blackN),
	blackKingN(pieces.blackKingN),
	whiteKingN(pieces.whiteKingN) {}