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

void Checkers::run(Color turnColor) {
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


	fout.close();
}
