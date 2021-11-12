#include "Checkers.h"

Checkers::Checkers(const std::string& filename) {
	fromFile(filename);
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
				whiteCheckers.emplace_back(std::shared_ptr<Figure>(figure));
				board[pos.getX()][pos.getY()] = figure;

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
				blackCheckers.emplace_back(std::shared_ptr<Figure>(figure));
				board[pos.getX()][pos.getY()] = figure;

				if (placementState == -1) placementState = (pos.getY() + pos.getX()) % 2;
				else if (placementState != (pos.getY() + pos.getX()) % 2)
					throw CheckersException();
			}
		}
	}

	fin.close();
}

void Checkers::run(Color _turnColor) {
	turnColor = _turnColor;
	//Moves moves;

	while (true) {
		Man* mptr = dynamic_cast<Man*>(&*std::move(whiteCheckers[1]));
		Figure* qptr = dynamic_cast<Figure*>(new Queen(std::move(*mptr)));
		whiteCheckers[1].reset(qptr);

		if (turnColor == Color::WHITE) {
			whiteCheckers[1]->possibleMoves(board);
		}
		else {

		}

		if (blackCheckers.size() == 0 || whiteCheckers.size() == 0)
			break;

		turnColor = (turnColor == Color::BLACK) ? Color::WHITE : Color::BLACK;
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

void Checkers::move(Position position, std::vector<Position>& moves) {
	auto* checkers = board[position.getX()][position.getY()]->getColor() == Color::WHITE ? &whiteCheckers : &blackCheckers;
	auto px = position.getX(), py = position.getY();

	for (const auto& move : moves) {
		auto mx = move.getX(), my = move.getY();
		board[mx][my] = board[px][py];
		board[px][py] = nullptr;

		auto oy = (my > py) ? py + ((my - py) >> 1) : my + ((py - my) >> 1);
		auto ox = (mx > px) ? px + ((mx - px) >> 1) : mx + ((px - mx) >> 1);

		size_t i = 0;
		for (i; i < checkers->size(); i++) {
			if (&*(*checkers)[i] == board[ox][oy]) {
				break;
			}
		}

		board[ox][oy] = nullptr;
		(*checkers)[i].swap((*checkers).back());
		checkers->pop_back();
	}

	this->moves.emplace_back(std::move(moves));
}

int Checkers::score() const {
	return blackCheckers.size() - whiteCheckers.size();
}
