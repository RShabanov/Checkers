#include "Checkers.h"

Checkers::Checkers(const std::string& filename)
	: firstMove(Color::WHITE) {
	fromFile(filename);
	board.state = { Color::WHITE, GameState::STILL_PLAYING };
}

void Checkers::fromFile(const std::string& filename) {
	std::ifstream fin(filename);

	if (!fin.is_open()) throw CheckersException();
	
	std::string buff;
	getline(fin, buff);

	switch (std::tolower(buff.front())) {
	case 'w':
		board.state.turnColor = Color::WHITE;
		break;
	case 'b': 
		board.state.turnColor = Color::BLACK;
		break;
	default:
		throw CheckersException();
	}

	auto whiteRegex = std::regex("white:\\s*(\\d+)", std::regex_constants::icase);
	auto blackRegex = std::regex("black:\\s*(\\d+)", std::regex_constants::icase);
	std::smatch regexMatch;

	Figure* figure = nullptr;
	Color color = Color::WHITE;
	auto checkers = &board.white; // default values

	auto placementState = CheckersPlacement::UNDEFINED;

	while (!fin.eof()) {
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

Color Checkers::currentColor() const {
	return board.state.turnColor;
}

void Checkers::run(Color turnColor) {
	runNMoves(-1, turnColor);
}

void Checkers::runNMoves(unsigned int moveNumber, Color turnColor) {
	int depth = moveNumber + 1;

	if (moveNumber == -1) depth = 4;

	firstMove = turnColor;
	board.state.turnColor = turnColor;

	auto checkers = (board.state.turnColor == Color::BLACK) ? &board.white : &board.black;

	// FOR DEBUG
	/*std::cout << board << std::endl;
	std::cin.get();

	while (board.state.state == GameState::STILL_PLAYING) {
		auto [score, newBoard] = minimax(board, depth, board.state.turnColor == Color::WHITE);
		//auto [score, newBoard] = alphaBeta(board, depth, 
		//	-std::numeric_limits<double>::infinity(), // alpha
		//	std::numeric_limits<double>::infinity(), // beta
		//	board.state.turnColor == Color::WHITE);

		board = std::move(newBoard);
		history.emplace_back(board.history);

		// FOR DEBUG
		/*std::cin.get();
		system("cls");
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

	fout << "The " << (firstMove == Color::WHITE ? "whites" : "blacks") << " starts." << std::endl;
	if (state() == GameState::STILL_PLAYING)
		fout << "Game is not over." << std::endl;
	else 
		fout << "The " << (firstMove == Color::WHITE ? "whites" : "blacks") << " won." << std::endl;

	fout << "History:\n";
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
	bool whiteOpponent = color != Color::WHITE;
	auto opponentSize = whiteOpponent ? board.white.size() : board.black.size();
	auto* checkers = whiteOpponent ? &board.black : &board.white;
	bool haveKillMove = false;
	std::vector<Board> moves;

	for (const auto& figure : *checkers) {
		for (const auto& move : figure->possibleMoves(board)) {

			auto tempBoard = boardCopy(board);
			tempBoard.history.emplace_back(figure->getPosition());
			
			// makes move
			simulateMove(&tempBoard, figure->getPosition(), move);

			auto newOpponentSize = whiteOpponent ? tempBoard.white.size() : tempBoard.black.size();
			if (opponentSize != newOpponentSize)
				haveKillMove = true;

			// add this move to all moves
			moves.emplace_back(std::move(tempBoard));
		}
	}

	// if we have kill moves we have to choose them
	if (haveKillMove) {
		auto removeStart = std::remove_if(moves.begin(), moves.end(),
			[opponentSize, whiteOpponent](const Board& _board) {
				return opponentSize == (whiteOpponent ? _board.white.size() : _board.black.size());
			});

		moves.erase(removeStart, moves.end());
	}

	auto comparator = [](
		const Board& lhs,
		const Board& rhs) {
			return lhs.score() == rhs.score() ? 
				lhs.history.size() > rhs.history.size() : 
				lhs.score() > rhs.score();
	};
	std::sort(moves.begin(), moves.end(), comparator);
	return std::move(moves);
}

void Checkers::simulateMove(Board* board, Position position, const std::vector<Position>& move) {
	for (const auto& step : move) {
		moveFigure(board, position, step);
		position = step;
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

			if (maxScore < score) {
				bestMove = std::move(newBoard);
				maxScore = score;
			}
		}

		// if moves empty an opponent wins
		if (moves.empty())
			bestMove.changeGameState(bestMove.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { maxScore, std::move(bestMove) };
	}
	else {
		double minScore = std::numeric_limits<double>::infinity();

		auto moves = getAllMoves(board, Color::BLACK);
		for (auto& newBoard : moves) {
			auto [score, _] = minimax(newBoard, depth - 1, true);

			if (minScore > score) {
				bestMove = std::move(newBoard);
				minScore = score;
			}
		}

		// if moves empty an opponent wins
		if (moves.empty())
			bestMove.changeGameState(bestMove.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { minScore, std::move(bestMove) };
	}
}

std::pair<double, Board> Checkers::alphaBeta(Board& board, int depth, double alpha, double beta, bool whiteTurn) {
	if (depth == 0 || board.state.state != GameState::STILL_PLAYING) {
		return { board.score(), board };
	}

	Board bestMove = board;

	if (whiteTurn) {
		double maxScore = -std::numeric_limits<double>::infinity();
		auto moves = getAllMoves(board, Color::WHITE);
		for (auto& newBoard : moves) {
			auto [score, _] = alphaBeta(newBoard, depth - 1, alpha, beta, false);

			if (maxScore < score) {
				bestMove = std::move(newBoard);
				maxScore = score;
			}

			alpha = alpha > score ? alpha : score;
			if (beta <= alpha) break;
		}

		// if moves empty an opponent wins
		if (moves.empty())
			board.changeGameState(bestMove.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { maxScore, std::move(bestMove) };
	}
	else {
		double minScore = std::numeric_limits<double>::infinity();

		auto moves = getAllMoves(board, Color::BLACK);
		for (auto& newBoard : moves) {
			auto [score, _] = alphaBeta(newBoard, depth - 1, alpha, beta, true);

			if (minScore > score) {
				bestMove = std::move(newBoard);
				minScore = score;
			}

			beta = beta < score ? beta : score;
			if (beta <= alpha) break;
		}

		// if moves empty an opponent wins
		if (moves.empty())
			board.changeGameState(bestMove.state.turnColor == Color::WHITE ? GameState::BLACK_WON : GameState::WHITE_WON);
		return { minScore, std::move(bestMove) };
	}
}
