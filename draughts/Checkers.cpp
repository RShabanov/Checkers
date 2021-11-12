#include "Checkers.h"

Checkers::Checkers(const std::string& filename) {
	fromFile(filename);
	board.state = { Color::WHITE, GameState::STILL_PLAYING };
}

void Checkers::fromFile(const std::string& filename) {
	std::ifstream fin(filename);

	if (!fin.is_open()) throw CheckersException();
	
	auto whiteRegex = std::regex("white:\\s*(\\d+)", std::regex_constants::icase);
	auto blackRegex = std::regex("black:\\s*(\\d+)", std::regex_constants::icase);
	std::smatch regexMatch;

	Figure* figure = nullptr;

	// responsable for placement of figures:
	// -1 - undefined
	// 0 - even
	// 1 - odd
	char placementState = -1;

	while (!fin.eof()) {
		std::string buff;
		getline(fin, buff);
		
		if (std::regex_match(buff, regexMatch, whiteRegex)) {
			for (int i = 0; i < std::stoi(regexMatch[1]) && !fin.eof(); i++) {
				getline(fin, buff);
				Position pos(buff);

				figure = dynamic_cast<Figure*>(new Man(pos, Color::WHITE));
				board.white.emplace_back(std::shared_ptr<Figure>(figure));
				board[pos] = figure;

				if (placementState == -1) placementState = (pos.getY() + pos.getX()) % 2;
				else if (placementState != (pos.getY() + pos.getX()) % 2)
					throw CheckersException();
			}
		}
		else if (std::regex_match(buff, regexMatch, blackRegex)) {
			for (int i = 0; i < std::stoi(regexMatch[1]) && !fin.eof(); i++) {
				getline(fin, buff);
				Position pos(buff);

				figure = dynamic_cast<Figure*>(new Man(pos, Color::BLACK));
				board.black.emplace_back(std::shared_ptr<Figure>(figure));
				board[pos] = figure;

				if (placementState == -1) placementState = (pos.getY() + pos.getX()) % 2;
				else if (placementState != (pos.getY() + pos.getX()) % 2)
					throw CheckersException();
			}
		}
	}

	fin.close();
}

void Checkers::run(Color _turnColor) {
	board.state.turnColor = _turnColor;

	auto checkers = (board.state.turnColor == Color::BLACK) ? &board.white : &board.black;

	while (board.state.state == GameState::STILL_PLAYING) {
		auto [score, newBoard] = minimax(board, 2, board.state.turnColor == Color::WHITE);

		board = std::move(newBoard);
		history.emplace_back(board.history);

		board.changeTurn();
	}
}

void Checkers::save(const std::string& filename) const {
	std::ofstream fout(filename);

	if (!fout.is_open()) throw CheckersException();

	for (size_t i = 0; i < history.size(); i++) {
		for (size_t moveIdx = 0; moveIdx < history[i].size() - 1; moveIdx++) {
			fout << history[i][moveIdx] << " -> ";
		}
		fout << history[i].back() << std::endl;
	}

	fout.close();
}

std::vector<Board> Checkers::getAllMoves(const Board& board, Color color) {
	auto* checkers = (color == Color::WHITE) ? &board.white : &board.black;
	std::vector<Board> moves;

	for (const auto& figure : *checkers) {
		for (const auto& move : figure->possibleMoves(board)) {

			auto tempBoard = createBoard(board);

			tempBoard.state.state = GameState::STILL_PLAYING;
			tempBoard.state.turnColor = figure->getColor();

			tempBoard.history.emplace_back(figure->getPosition());
			simulateMove(&tempBoard, figure->getPosition(), move);

			moves.emplace_back(std::move(tempBoard));
		}
	}
	return std::move(moves);
}

void Checkers::simulateMove(Board* board, Position position, const std::vector<Position>& move) {
	auto removeBlackColor = (*board)[position]->getColor() == Color::WHITE;

	auto* checkers = removeBlackColor ? &board->black : &board->white;

	for (const auto& step : move) {
		board->moveFigure(position, step);

		if ((*board)[step]->getColor() == Color::WHITE) {
			if (step.getY() == BOARD_SIZE - 1) {
				for (size_t i = 0; i < board->white.size(); i++) {
					if (&*(board->white)[i] == (*board)[step]) {
						Figure* queen = dynamic_cast<Figure*>(new Queen(board->white[i]->getPosition(), board->white[i]->getColor()));
						board->white[i].reset(queen);
						(*board)[step] = queen;
						board->increaseQueen(Color::WHITE);

						break;
					}
				}
			}
		}
		else {
			if (step.getY() == 0) {
				for (size_t i = 0; i < board->black.size(); i++) {
					if (&*(board->black)[i] == (*board)[step]) {
						Figure* queen = dynamic_cast<Figure*>(new Queen(board->black[i]->getPosition(), board->black[i]->getColor()));
						board->black[i].reset(queen);
						(*board)[step] = queen;
						board->increaseQueen(Color::BLACK);

						break;
					}
				}
			}
		}

		auto [hasOpponent, opponent] = board->between(step, position, removeBlackColor ? Color::BLACK : Color::WHITE);

		// |x1 - x2| + |y1 - y2|
		if (hasOpponent) {
			size_t i = 0;
			for (i; i < checkers->size(); i++) {
				if (&*(*checkers)[i] == (*board)[opponent]) {
					break;
				}
			}

			(*board)[opponent] = nullptr;
			(*checkers)[i].swap((*checkers).back());
			checkers->pop_back();

			if (checkers->empty())
				board->state.state = removeBlackColor ? GameState::WHITE_WON : GameState::BLACK_WON;
		}

		position = step;
		board->history.emplace_back(std::move(step));
	}
}


std::pair<double, Board> Checkers::minimax(Board& board, int depth, bool whiteTurn) {
	if (depth == 0 || board.state.state != GameState::STILL_PLAYING) {
		return { board.score(), board };
	}

	Board bestMove = board;

	if (whiteTurn) {
		double maxScore = -std::numeric_limits<double>::infinity();
		auto moves = getAllMoves(board, Color::WHITE);
		for (auto& newBoard : moves) {
			auto [score, _] = minimax(newBoard, depth - 1, false);
			maxScore = maxScore > score ? maxScore : score;

			if (maxScore == score) bestMove = std::move(newBoard);

		}
		if (moves.empty())
			board.changeGameState(board.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { maxScore, std::move(bestMove) };
	}
	else {
		double minScore = std::numeric_limits<double>::infinity();

		auto moves = getAllMoves(board, Color::BLACK);
		for (auto& newBoard : moves) {
			auto [score, _] = minimax(newBoard, depth - 1, true);
			minScore = minScore < score ? minScore : score;

			if (minScore == score) bestMove = std::move(newBoard);

		}
		if (moves.empty())
			board.changeGameState(board.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { minScore, std::move(bestMove) };
	}
}


Board Checkers::createBoard(const Board& board) const {
	Board newBoard(board.state, board.getBlackQueenN(), board.getWhiteQueenN());

	for (const auto& white : board.white) {
		Figure* ptr = nullptr;
		if (white->isQueen())
			ptr = dynamic_cast<Figure*>(new Queen(Position(white->getX(), white->getY()), white->getColor()));
		else 
			ptr = dynamic_cast<Figure*>(new Man(Position(white->getX(), white->getY()), white->getColor()));

		newBoard.white.emplace_back(std::shared_ptr<Figure>(ptr));
		newBoard[white->getPosition()] = ptr;
	}

	for (const auto& black : board.black) {
		Figure* ptr = nullptr;
		if (black->isQueen())
			ptr = dynamic_cast<Figure*>(new Queen(Position(black->getX(), black->getY()), black->getColor()));
		else
			ptr = dynamic_cast<Figure*>(new Man(Position(black->getX(), black->getY()), black->getColor()));

		newBoard.black.emplace_back(std::shared_ptr<Figure>(ptr));
		newBoard[black->getPosition()] = ptr;
	}

	return std::move(newBoard);
}