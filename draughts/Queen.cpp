#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Queen::Queen(Man&& man)
	: Figure(man) {}

Moves Queen::possibleMoves(const Board& board) const {
	Moves moves;
	constexpr int QUEEN_DEPTH = 3;

	moves = getPossibleAttacks(board, position);

	// at least, if don't have any possible attacks
	// we can jump to cell where we can attack from
	if (moves.empty()) {
		for (int step = 1; step < BOARD_SIZE - 1; step++) {
			for (int signX = -1; signX < 2; signX += 2) {
				if (!board.onBoard(step * signX, 0)) continue;

				for (int signY = -1; signY < 2; signY += 2) {
					if (!board.onBoard(step * signX, step * signY)) continue;

					Position from(signX * step, signY * step);

					if (canAttackFrom(board, from)) {
						moves.emplace_back(std::vector<Position>(1, from));
					}
				}
			}
		}
	}

	return std::move(moves);
}

Moves Queen::getPossibleAttacks(const Board& board, const Position& pos) const {
	std::vector<Position> oneLineAttacks;
	Moves attacks;
	bool gotLine = false;

	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = pos.getY() + y;
			int nx = pos.getX() + x;

			while (board.onBoard(nx, ny)) {

				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color) break;
					else {
						nx += x;
						ny += y;

						if (!board.onBoard(nx, ny)) break;

						if (board.isEmpty(nx, ny)) {
							gotLine = true;
							oneLineAttacks.emplace_back(Position(nx, ny));
						}
						else break;
					}

				}

				nx += x;
				ny += y;
			}

			if (gotLine) {
				ny = pos.getY() + y;
				nx = pos.getX() + x;

				while (!(oneLineAttacks.back() == Position(nx, ny)) &&
					nx > 0 && nx < BOARD_SIZE &&
					ny > 0 && ny < BOARD_SIZE) {

					if (board.isEmpty(nx - x, ny - y) && 
						board.onBoard(nx - x * 2, ny - y * 2)) {

						if (board.isEmpty(nx - x*2, ny - y*2) ||
							board.data[nx - x * 2][ny - y * 2]->getColor() != color)
							oneLineAttacks.emplace_back(Position(nx - x, ny - y));
					}
					nx += x;
					ny += y;
				}

				attacks.emplace_back(std::move(oneLineAttacks));
				oneLineAttacks.clear();
				gotLine = false;
			}
		}
	}

	return attacks;
}

bool Queen::canAttackFrom(const Board& board, const Position& from) const {
	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = from.getY() + y;
			int nx = from.getX() + x;

			while (board.onBoard(nx, ny)) {

				if (!board.isEmpty(nx, ny)) {
					if (board.data[nx][ny]->getColor() == color) break;

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
