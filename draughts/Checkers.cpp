#include "Checkers.h"

Checkers::Checkers(const std::string& filename) {
	fromFile(filename);
	board.state = { Color::WHITE, GameState::STILL_PLAYING };
}

Checkers::~Checkers() {
	std::cout << "Everything deleted\n";
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
				board.data[pos.getX()][pos.getY()] = figure;

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
				board.data[pos.getX()][pos.getY()] = figure;

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
	//Moves moves;
	auto checkers = (board.state.turnColor == Color::BLACK) ? &board.white : &board.black;

	while (board.state.state == GameState::STILL_PLAYING) {
		auto [score, newBoard] = minimax(createBoard(board), 3, board.state.turnColor == Color::WHITE);

		board = std::move(newBoard);

		board.changeTurn();
		checkers = (board.state.turnColor == Color::BLACK) ? &board.white : &board.black;
	}

}

void Checkers::save(const std::string& filename) const {
	std::ofstream fout(filename);

	if (!fout.is_open()) throw CheckersException();

	for (size_t i = 0; i < moves.size(); i++) {
		for (size_t moveIdx = 0; moveIdx < moves[i].size() - 1; moveIdx++) {
			fout << moves[i][moveIdx] << " -> ";
		}
		fout << moves[i].back() << std::endl;
	}

	fout.close();
}

std::vector<Board> Checkers::getAllMoves(const Board& board, Color color) {
	auto* checkers = (color == Color::WHITE) ? &board.white : &board.black;
	std::vector<Board> moves;

	for (const auto& figure : *checkers) {
		for (const auto& move : figure->possibleMoves(board)) {

			auto tempBoard = createBoard(board);

			simulateMove(&tempBoard, figure->getPosition(), move);
			moves.emplace_back(std::move(tempBoard));
		}
	}
	return std::move(moves);
}

std::vector<Position> Checkers::getBestMove(Color color) {
	return std::vector<Position>();
}

void Checkers::simulateMove(Board* board, Position position, const std::vector<Position>& move) {
	auto* checkers = board->data[position.getX()][position.getY()]->getColor() == Color::WHITE ? &board->black : &board->white;
	auto px = position.getX(), py = position.getY();

	for (const auto& step : move) {
		auto mx = step.getX(), my = step.getY();
		board->data[mx][my] = board->data[px][py];
		board->data[px][py] = nullptr;

		if (board->data[mx][my]->getColor() == Color::WHITE) {
			if (mx == BOARD_SIZE - 1) {
				for (size_t i = 0; i < board->white.size(); i++) {
					if (&*(board->white)[i] == board->data[mx][my]) {
						Figure* queen = dynamic_cast<Figure*>(new Queen(board->white[i]->getPosition(), board->white[i]->getColor()));
						board->white[i].reset(queen);
						break;
					}
				}
			}
		}
		else {
			if (mx == 0) {

			}
		}

		auto oy = (my > py) ? py + ((my - py) >> 1) : my + ((py - my) >> 1);
		auto ox = (mx > px) ? px + ((mx - px) >> 1) : mx + ((px - mx) >> 1);

		size_t i = 0;
		for (i; i < checkers->size(); i++) {
			if (&*(*checkers)[i] == board->data[ox][oy]) {
				break;
			}
		}

		board->data[ox][oy] = nullptr;
		(*checkers)[i].swap((*checkers).back());
		checkers->pop_back();
	}
}


std::pair<double, Board> Checkers::minimax(const Board& board, int depth, bool max) {
	if (depth == 0 || board.state.state != GameState::STILL_PLAYING) {
		return { board.score(), board };
	}

	Board bestMove;

	if (max) {
		double maxScore = DBL_MAX;

		for (const auto& newBoard : getAllMoves(board, Color::WHITE)) {
			auto [score, position] = minimax(newBoard, depth - 1, false);
			maxScore = maxScore > score ? maxScore : score;

			if (maxScore == score) bestMove = std::move(newBoard);

		}
		return { maxScore, std::move(bestMove) };
	}
	else {
		double minScore = DBL_MIN;

		for (const auto& newBoard : getAllMoves(board, Color::BLACK)) {
			auto [score, position] = minimax(newBoard, depth - 1, true);
			minScore = minScore > score ? minScore : score;

			if (minScore == score) bestMove = std::move(newBoard);

		}
		return { minScore, std::move(bestMove) };
	}
}


Board Checkers::createBoard(const Board& board) const {
	Board newBoard;
	std::vector<std::shared_ptr<Figure>> newBlacks, newWhites;

	for (const auto& white : board.white) {
		Figure* ptr = nullptr;
		if (white->isQueen())
			ptr = dynamic_cast<Figure*>(new Queen(Position(white->getX(), white->getY()), white->getColor()));
		else 
			ptr = dynamic_cast<Figure*>(new Man(Position(white->getX(), white->getY()), white->getColor()));

		newWhites.emplace_back(std::shared_ptr<Figure>(ptr));
		newBoard.data[white->getX()][white->getY()] = ptr;
	}

	for (const auto& black : board.black) {
		Figure* ptr = nullptr;
		if (black->isQueen())
			ptr = dynamic_cast<Figure*>(new Queen(Position(black->getX(), black->getY()), black->getColor()));
		else
			ptr = dynamic_cast<Figure*>(new Man(Position(black->getX(), black->getY()), black->getColor()));

		newBlacks.emplace_back(std::shared_ptr<Figure>(ptr));
		newBoard.data[black->getX()][black->getY()] = ptr;
	}


	return newBoard;
}