#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Queen::Queen(Man&& man)
	: Figure(man) {}

Moves Queen::possibleMoves(const Board& board) const {
	Moves moves;
	constexpr int QUEEN_DEPTH = 3;

	moves = getPossibleAttacks(board, position);
	
	// TODO: finish chain move

	return moves;
}

Moves Queen::getPossibleAttacks(const Board& board, const Position& pos) const {
	std::vector<Position> oneLineAttacks;
	Moves attacks;
	bool gotLine = false;

	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int	ny = pos.getY() + y;
			int nx = pos.getX() + x;

			if (ny < 0 || ny >= BOARD_SIZE ||
				nx < 0 || nx >= BOARD_SIZE)
				continue;

			while ((nx >= 0 && nx < BOARD_SIZE) &&
				(ny >= 0 && ny < BOARD_SIZE)) {

				if (board[nx][ny] != nullptr) {
					if (board[nx][ny]->getColor() == color) break;
					else {
						nx += x;
						ny += y;

						if ((nx < 0 || nx >= BOARD_SIZE) ||
							(ny < 0 || ny >= BOARD_SIZE)) break;

						if (board[nx][ny] == nullptr) {
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

					if (board[nx - x][ny - y] == nullptr) {
						if (!(board[nx - x * 2][ny - y * 2] != nullptr &&
							board[nx - x * 2][ny - y * 2]->getColor() != color))
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

bool Queen::isQueen() const { return true; }
