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
	Color color = Color::WHITE;
	auto checkers = &board.white; // default values

	auto placementState = CheckersPlacement::UNDEFINED;

	while (!fin.eof()) {
		std::string buff;
		getline(fin, buff);

		if (std::regex_match(buff, regexMatch, whiteRegex)) {
			color = Color::WHITE;
			checkers = &board.white;
		}
		else if (std::regex_match(buff, regexMatch, blackRegex)) {
			color = Color::BLACK;
			checkers = &board.black;
		}
		else throw CheckersException();

		for (int i = 0; i < std::stoi(regexMatch[1]) && !fin.eof(); i++) {
			Position pos(0, 0); // just to fill with some values
			getline(fin, buff);
			
			// if buff starts with 'M' we have Queen
			// otherwise it is Man
			bool isQueen = std::toupper(buff.front()) == 'M';
			if (isQueen) {
				pos.set(buff.substr(1));
				figure = dynamic_cast<Figure*>(new Queen(pos, color));
			}
			else {
				pos.set(buff);
				figure = dynamic_cast<Figure*>(new Man(pos, color));
			}

			checkers->emplace_back(std::shared_ptr<Figure>(figure));
			board[pos] = figure;

			auto placement = (pos.getY() + pos.getX()) % 2 ? CheckersPlacement::ODD : CheckersPlacement::EVEN;
			if (placementState == CheckersPlacement::UNDEFINED)
				placementState = placement;
			else if (placementState != placement)
				throw CheckersException();
		}
	}

	fin.close();
}

void Checkers::run(Color turnColor) {
	runNMoves(-1, turnColor);
}

void Checkers::runNMoves(unsigned int moveNumber, Color turnColor) {
	int depth = moveNumber + 1;

	if (moveNumber == -1) depth = 4;

	board.state.turnColor = turnColor;

	auto checkers = (board.state.turnColor == Color::BLACK) ? &board.white : &board.black;

	// FOR DEBUG
	/*
	std::cout << board << std::endl;
	*/

	while (board.state.state == GameState::STILL_PLAYING) {
		auto [score, newBoard] = minimax(board, depth, board.state.turnColor == Color::WHITE);

		board = std::move(newBoard);
		history.emplace_back(board.history);

		// FOR DEBUG
		/*system("cls");
		std::cout << board << std::endl;
		board.printHistory(std::cout);*/

		board.changeTurn();

		if (moveNumber != -1) {
			if (moveNumber-- == 0) {
				break;
			}
		}
	}
}

GameState Checkers::state() const {
	return board.state.state;
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

// in spite of function name 
// it returns boards where these moves are done
std::vector<Board> Checkers::getAllMoves(const Board& board, Color color) {
	auto* checkers = (color == Color::WHITE) ? &board.white : &board.black;
	std::vector<Board> moves;

	for (const auto& figure : *checkers) {
		for (const auto& move : figure->possibleMoves(board)) {

			auto tempBoard = createBoard(board);
			tempBoard.history.emplace_back(figure->getPosition());
			
			// makes move
			simulateMove(&tempBoard, figure->getPosition(), move);

			// add this move to all moves
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

		// checks if a figure becomes a queen
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

		// checks if it is a kill move and returns position of killed figure 
		auto [hasOpponent, opponent] = board->between(step, position, removeBlackColor ? Color::BLACK : Color::WHITE);

		// if it is a kill move
		if (hasOpponent) {
			size_t i = 0;
			for (i; i < checkers->size(); i++) {
				if (&*(*checkers)[i] == (*board)[opponent]) {
					break;
				}
			}

			// removes killed figure
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

// main algorithm (AI)
// score function: len(white) - len(black) + whiteQueenN * 0.5 - blackQueenN * 0.5
// so for the whites it has to be MAX
// for the blacks - MIN
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

		// if moves empty an opponent wins
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

		// if moves empty an opponent wins
		if (moves.empty())
			board.changeGameState(board.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { minScore, std::move(bestMove) };
	}
}


Board Checkers::createBoard(const Board& board) const {
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