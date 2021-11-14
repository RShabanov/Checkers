#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Moves Queen::possibleMoves(const Board& board, bool onlyAttack) const {
	Moves moves;

	moves = getPossibleAttacks(const_cast<Board&>(board), position);

	// at least, if don't have any possible attacks
	// we can jump to cell where we can attack from
	if (moves.empty() && !onlyAttack) {
		auto maxValidStep = BOARD_SIZE - (position.getX() > position.getY() ? position.getX() : position.getY());

		for (int signX = -1; signX < 2; signX += 2) {
			for (int signY = -1; signY < 2; signY += 2) {
				for (int step = 1; step <= maxValidStep; step++) {
					int nx = position.getX() + step * signX,
						ny = position.getY() + step * signY;

					if (!board.onBoard(nx, ny) || !board.isEmpty(nx, ny)) 
						break;

					moves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
				}
			}
		}
	}

	return std::move(moves);
}

Moves Queen::getPossibleAttacks(Board& board, const Position& pos) const {
	Moves attacks;

	// checks neighbourhood:
	// (-n, -n) . (-n, n)
	//	   .	o	 .
	// ( n,  n) . ( n, n)
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = pos.getY() + y;
			int nx = pos.getX() + x;

			while (board.onBoard(nx, ny)) {

				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color) break;
					else {
						// if we found an opponent
						// we have to kill it and since remove it
						Position opponentPosition(nx, ny);
						Figure* const tempOpponentStorage = board[opponentPosition];

						do {
							nx += x;
							ny += y;

							if (!board.onBoard(nx, ny)) break;

							if (board.isEmpty(nx, ny)) {
								Position attackPosition(nx, ny);
								board[opponentPosition] = nullptr; // "remove"

								// continue kill chain if it is possible
								auto possibleMovesFrom = getPossibleAttacks(board, attackPosition);

								// nothing can be attacked from attackPosition
								if (possibleMovesFrom.empty()) { 
									attacks.emplace_back(std::vector<Position>(1, attackPosition));
								}
								else {
									// otherwise we have different ways to kill
									auto startIdx = attacks.size();

									for (size_t i = startIdx; i < possibleMovesFrom.size(); i++) {
										// add start position
										attacks.emplace_back(std::vector<Position>(1, attackPosition));

										auto possibleMovesIdx = startIdx > 0 ? i % startIdx : i;

										// add the rest
										attacks[i].insert(attacks[i].end(),
											possibleMovesFrom[possibleMovesIdx].begin(),
											possibleMovesFrom[possibleMovesIdx].end());
									}
								}
							}
							else break;
						} while (true);

						// since we don't copy board
						// and we "remove" opponents
						// we have to move it back
						board[opponentPosition] = tempOpponentStorage;
					}
				}

				nx += x;
				ny += y;
			}
		}
	}

	return attacks;
}

bool Queen::canAttackFrom(const Board& board, const Position& from) const {
	// checks neighbourhood:
	// (-n, -n) . (-n, n)
	//	   .	o	 .
	// ( n,  n) . ( n, n)
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = from.getY() + y;
			int nx = from.getX() + x;

			while (board.onBoard(nx, ny)) {
				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color ||
						board.data[nx][ny]->isQueen()) break;

					nx += x;
					ny += y;

					if (!board.onBoard(nx, ny)) break;

					if (board.isEmpty(nx, ny)) return true;
				}

				nx += x;
				ny += y;
			}
		}
	}
	return false;
}

bool Queen::isQueen() const { return true; }
