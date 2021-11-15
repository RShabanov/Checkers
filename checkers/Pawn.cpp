#include "Pawn.h"

Pawn::Pawn(const Position& _position, Color _color) 
	: Piece(_position, _color) {}

Moves Pawn::possibleMoves(const GameBoard& board, const Position& from, bool onlyAttack) const {
	Moves killMoves, usualMoves;

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

			Position to(nx, ny);

			// if the current cell is not empty
			if (!board.isEmpty(to)) {
				// if it is not an opponent
				if (board[to]->getColor() == color) continue;

				// if it is an opponent
				nx += x;
				ny += y;

				// if the cell after the current cell is empty
				if (board.onBoard(nx, ny) &&
					board.isEmpty(nx, ny)) {
					to.set(ny, nx);
					Position opponentPosition(nx - x, ny - y);

					killMoves.emplace_back(std::vector<Position>(1, to));

					// since we can kill an opponent
					// we have to create a new board not to affect on the original one
					GameBoard newBoard(copyBoard(board));
					movePiece(&newBoard, from, to);

					// if a pawn is still a pawn then
					// it generates a kill chain as a pawn
					// otherwise it generates a kill chain as a king
					auto moves = newBoard[to]->possibleMoves(newBoard, to, newBoard[to]->isKing());
					
					// TODO: implement functions: copyBoard & movePiece
					for (size_t moveIdx = 0; moveIdx < moves.size(); moveIdx++) {
						killMoves.back().insert(killMoves.back().end(), moves[moveIdx].begin(), moves[moveIdx].end());
						if (moveIdx != moves.size() - 1)
							killMoves.emplace_back(std::vector<Position>(1, to));
					}
				}

				nx -= x;
			}
			// if we have something to kill
			// we've got to kill it
			else if (onlyAttack && !killMoves.empty()) {
				// since the whites are below the black ones
				if ((color == Color::WHITE && y < 0) ||
					(color == Color::BLACK && y > 0))
					continue;

				usualMoves.emplace_back(std::vector<Position>(1, to));
			}
		}
	}

	return killMoves.empty() ? std::move(usualMoves) : std::move(killMoves);
}

bool Pawn::isKing() const { return false; }
