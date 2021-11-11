#include "Man.h"

Man::Man(const Position& position, Color color)
	: Figure(position, color) {}

Moves Man::possibleMoves(const Board& board) const {
	Moves moves;
	bool hasEatChain = false;

	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int nx = position.getX() + x,
				ny = position.getY() + y;

			if (nx < 0 || nx >= BOARD_SIZE ||
				ny < 0 || ny >= BOARD_SIZE)
				continue;

			// if the current cell is not empty
			if (board[nx][ny] != nullptr) {

				// if the cell after the current cell is empty
				if (board[nx][ny]->getColor() != color) {
					nx += x;
					ny += y;

					if (board[nx][ny] == nullptr) {
						moves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
						eatMove(board, Position(nx, ny), Position(nx - x, ny - y), &moves, moves.size() - 1);

						hasEatChain = true;
					}
				}
			}
			else {
				// since the whites are below the black ones
				if ((color == Color::WHITE && y > 0) ||
					(color == Color::BLACK && y < 0))
					continue;

				hasEatChain = false;
				moves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
			}
		}
	}

	if (hasEatChain) {
		moves.erase(std::remove_if(
			moves.begin(), moves.end(),
			[](const std::vector<Position>& chain) {
				return chain.size() == 1;
			}),
			moves.end()
				);
	}

	return moves;
}

void Man::eatMove(
	const Board& board,
	Position current,
	Position opponent,
	Moves* moves,
	size_t idx
) const {
	size_t sizeBefore = -1;
	bool finishBranch = false;

	for (int x = -1; x < 2; x += 2) {
		for (int y = -1; y < 2; y += 2) {
			int nx = current.getX() + x,
				ny = current.getY() + y;

			if (nx < 0 || nx >= BOARD_SIZE ||
				ny < 0 || ny >= BOARD_SIZE ||
				Position(nx, ny) == opponent)
				continue;

			// if there is a neighbor
			if (board[nx][ny] != nullptr &&
				board[nx][ny]->getColor() != color) {

				nx += x;
				ny += y;

				if (board[nx][ny] == nullptr) {
					if (finishBranch) {
						finishBranch = false;

						moves->emplace_back(std::vector<Position>(sizeBefore, current));
						std::copy((*moves)[idx].begin(), (*moves)[idx].begin() + sizeBefore, moves->back().begin());
						idx += 1;
					}
					sizeBefore = (*moves)[idx].size();

					(*moves)[idx].emplace_back(Position(nx, ny));
					eatMove(board, Position(nx, ny), Position(nx - x, ny - y), moves, idx);

					finishBranch = true;
				}
			}
		}
	}
}

bool Man::isQueen() const {	return false; }
