#include "Pawn.h"

Pawn::Pawn(const Position& _position, Color _color) 
	: Piece(_position, _color) {}

Moves Pawn::possibleMoves(const GameBoard& board, const Position& from, bool onlyAttack) const {
	Moves killMoves, usualMoves;

	// Moves = std::vector<Move>
	// Move = std::vector<Position>
	// 
	// checks neighbourhood:
	// (-1, -1) . (-1, 1)
	//	   .	o	 .
	// ( 1,  1) . ( 1, 1)
	for (int y = -1; y < 2; y += 2) {
		int ny = from.getY() + y;
		if (!board.onBoard(ny, 0)) continue;

		for (int x = -1; x < 2; x += 2) {
			int nx = from.getX() + x;
			if (!board.onBoard(ny, nx)) continue;

			Position to(ny, nx);

			// if the current cell is not empty
			if (!board.isEmpty(to)) {
				// if it is not an opponent
				if (board[to]->getColor() == color) continue;

				Position opponentPosition(to);
				to.add(y, x);

				// if the cell after the current cell is empty
				if (board.onBoard(to) &&
					board.isEmpty(to)) {

					// since we can kill an opponent
					// we have to create a new board not to affect on the original one
					GameBoard newBoard(board.copy());
					movePiece(&newBoard, from, to, opponentPosition);

					// if a pawn is still a pawn then
					// it generates a kill chain as a pawn
					// otherwise it generates a kill chain as a king
					auto moves = newBoard[to]->possibleMoves(newBoard, to, true);
					
					if (moves.empty())
						killMoves.emplace_back(Move(1, to));

					for (const auto& move : moves) {
						killMoves.emplace_back(Move(1, to));
						killMoves.back().insert(killMoves.back().begin(),
							move.begin(), move.end());
					}
				}
			}
			// if we have something to kill
			// we've got to kill it
			else if (onlyAttack && !killMoves.empty()) {
				// since the whites are below the black ones
				if ((color == Color::WHITE && y < 0) ||
					(color == Color::BLACK && y > 0))
					continue;

				usualMoves.emplace_back(Move(1, to));
			}
		}
	}

	return killMoves.empty() ? std::move(usualMoves) : std::move(killMoves);
}

bool Pawn::isKing() const { return false; }
