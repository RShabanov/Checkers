#include "Man.h"

Man::Man(const Position& position, Color color)
	: Figure(position, color) {}

Moves Man::possibleMoves(const Board& board) const {
	Moves chainMoves, oneStepMoves;

	// checks neighbourhood:
	// (-1, -1) . (-1, 1)
	//	   .	o	 .
	// ( 1,  1) . ( 1, 1)
	for (int x = -1; x < 2; x += 2) {
		int nx = position.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = position.getY() + y;

			if (!board.onBoard(nx, ny))
				continue;

			// if the current cell is not empty
			if (!board.isEmpty(nx, ny)) {

				// if it is an opponent
				if (board.data[nx][ny]->getColor() != color) {
					nx += x;
					ny += y;

					// if the cell after the current cell is empty
					if (board.onBoard(nx, ny) && 
						board.isEmpty(nx, ny)) {
						Position current(nx, ny), 
								 opponentPosition(nx - x, ny - y);

						if (onQueenPositionIf(board, current))
							queenMove(board, current, chainMoves);
						else {
							chainMoves.emplace_back(std::vector<Position>(1, current));
							eatMove(board, current, opponentPosition, &chainMoves, chainMoves.size() - 1);
						}
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

	// if we have kill chain
	// we have to kill no matter what
	if (!chainMoves.empty()) return std::move(chainMoves);
	
	return std::move(oneStepMoves);
}

void Man::eatMove(
	const Board& board,
	Position current,
	Position opponent, // opponent position (right between original figure position and current one
	Moves* moves,
	size_t idx // index of the current move
) const {
	size_t sizeBefore = -1;
	bool finishBranch = false;

	// checks neighbourhood:
	// (-1, -1) . (-1, 1)
	//	   .	o	 .
	// ( 1,  1) . ( 1, 1)
	for (int x = -1; x < 2; x += 2) {
		int nx = current.getX() + x;
		if (!board.onBoard(nx, 0)) continue;

		for (int y = -1; y < 2; y += 2) {
			int	ny = current.getY() + y;

			if (!board.onBoard(nx, ny) ||
				Position(nx, ny) == opponent)
				continue;

			// if there is a neighbour
			if (!board.isEmpty(nx, ny) &&
				board.data[nx][ny]->getColor() != color) {

				nx += x;
				ny += y;

				if (board.onBoard(nx, ny) && board.isEmpty(nx, ny)) {

					// if there is a crossroad
					// we have to write one of the possible chains
					if (finishBranch) {
						finishBranch = false;

						moves->emplace_back(std::vector<Position>(sizeBefore, current));
						std::copy((*moves)[idx].begin(), (*moves)[idx].begin() + sizeBefore, moves->back().begin());

						// since we add finished chain
						// and we have another chain with another index
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

void Man::queenMove(const Board& board, const Position& current, Moves& chainMoves) const {
	Board newBoard = boardCopy(board);

	moveFigure(&newBoard, position, current);

	auto moves = newBoard[current]->possibleMoves(newBoard);
	if (moves.empty())
		chainMoves.emplace_back(std::vector<Position>(1, current));
	else {
		for (const auto& move : moves) {
			chainMoves.emplace_back(std::vector<Position>(1, current));
			chainMoves.back().insert(chainMoves.back().end(), move.begin(), move.end());
		}
	}
}

bool Man::onQueenPositionIf(const Board& board, const Position& pos) const {
	if (board.isEmpty(pos))
		return (color == Color::WHITE && pos.getY() == BOARD_SIZE - 1) ||
		(color == Color::BLACK && pos.getY() == 0);

	return false;
}

void moveFigure(Board* _board, const Position& from, const Position& to) {
	// variable shadowing (just not to write (*_board))
	Board& board = *_board;

	if (!board.onBoard(from) || !board.onBoard(to))
		throw BoardException();

	bool removeBlackColor = board[from]->isWhite();
	std::vector<std::shared_ptr<Figure>>* checkers = nullptr;

	board[to] = board[from];
	board[from] = nullptr;
	board[to]->setPosition(to);

	// checks if a figure becomes a queen
	bool isQueen = (board[to]->isWhite() && to.getY() == BOARD_SIZE - 1) ||
		(board[to]->isBlack() && to.getY() == 0);

	if (isQueen && !board[to]->isQueen()) {
		checkers = board[to]->isWhite() ? &board.white : &board.black;

		for (size_t figureIdx = 0; figureIdx < checkers->size(); figureIdx++)
			if ((*checkers)[figureIdx]->getPosition() == to) {
				Figure* queen = dynamic_cast<Figure*>(new Queen(to, board[to]->getColor()));

				board[to] = queen;
				(*checkers)[figureIdx].reset(queen);
				board.increaseQueen(board[to]->getColor());

				break;
			}
	}

	auto [hasOpponent, opponent] = board.between(to, from, removeBlackColor ? Color::BLACK : Color::WHITE);

	// if it is a kill move
	if (hasOpponent) {
		checkers = removeBlackColor ? &board.black : &board.white;
		size_t i = 0;
		for (i; i < checkers->size(); i++) {
			if (&*(*checkers)[i] == board[opponent]) {
				break;
			}
		}

		// removes killed figure
		board[opponent] = nullptr;
		(*checkers)[i].swap((*checkers).back());
		checkers->pop_back();

		if (checkers->empty())
			board.changeGameState(removeBlackColor ? GameState::WHITE_WON : GameState::BLACK_WON);
	}

	board.history.emplace_back(to);
}

bool Man::isQueen() const {	return false; }

bool Man::onQueenPosition() const {
	return isWhite() && position.getY() == BOARD_SIZE - 1 ||
		isBlack() && position.getY() == 0;
}

Board boardCopy(const Board& board) {
	Board newBoard(board.state, board.getBlackQueenN(), board.getWhiteQueenN());
	auto checkers = &board.white;
	auto newCheckers = &newBoard.white;

	for (int i = 0; i < 2; i++) {

		for (const auto& figure : (*checkers)) {
			Figure* ptr = nullptr;
			if (figure->isQueen())
				ptr = dynamic_cast<Figure*>(new Queen(figure->getPosition(), figure->getColor()));
			else
				ptr = dynamic_cast<Figure*>(new Man(figure->getPosition(), figure->getColor()));

			newCheckers->emplace_back(std::shared_ptr<Figure>(ptr));
			newBoard[figure->getPosition()] = ptr;
		}

		checkers = &board.black;
		newCheckers = &newBoard.black;
	}

	return std::move(newBoard);
}
