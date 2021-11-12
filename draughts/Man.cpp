#include "Man.h"

Man::Man(const Position& position, Color color)
	: Figure(position, color) {}

Moves Man::possibleMoves(const Board& board) const {
	Moves chainMoves, oneStepMoves;

	for (int x = -1; x < 2; x += 2) {
		int nx = position.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = position.getY() + y;

			if (!board.onBoard(nx, ny))
				continue;

			// if the current cell is not empty
			if (!board.isEmpty(nx, ny)) {

				// if the cell after the current cell is empty
				if (board.data[nx][ny]->getColor() != color) {
					nx += x;
					ny += y;

					if (!board.onBoard(nx, ny)) continue;

					if (board.isEmpty(nx, ny)) {
						chainMoves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
						eatMove(board, Position(nx, ny), Position(nx - x, ny - y), &chainMoves, chainMoves.size() - 1);
					}

					nx -= x;
					ny -= y;
				}
			}
			// if we have something to kill
			// we've got to kill it
			else if (chainMoves.empty()) {
				// since the whites are below the black ones
				if ((color == Color::WHITE && y < 0) ||
					(color == Color::BLACK && y > 0))
					continue;

				oneStepMoves.emplace_back(std::vector<Position>(1, Position(nx, ny)));
			}
		}
	}

	auto comparator = [](
		const std::vector<Position>& lhs,
		const std::vector<Position>& rhs) {
			return lhs.size() > rhs.size();
	};

	if (!chainMoves.empty()) {
		std::sort(chainMoves.begin(), chainMoves.end(), comparator);
		return std::move(chainMoves);
	}
	else {
		std::sort(oneStepMoves.begin(), oneStepMoves.end(), comparator);
		return std::move(oneStepMoves);
	}
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
		int nx = current.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = current.getY() + y;

			if (!board.onBoard(nx, ny) ||
				Position(nx, ny) == opponent)
				continue;

			// if there is a neighbor
			if (!board.isEmpty(nx, ny) &&
				board.data[nx][ny]->getColor() != color) {

				nx += x;
				ny += y;

				if (board.isEmpty(nx, ny)) {
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
