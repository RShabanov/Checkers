#include "Checkers.h"
#include "Man.h"

Checkers::Checkers(const std::string& filename) {
	fromFile(filename);
}

void Checkers::fromFile(const std::string& filename) {
	std::ifstream fin(filename);

	if (!fin.is_open()) throw CheckersException();
	
	auto whiteRegex = std::regex("white:\\s*(\\d+)", std::regex_constants::icase);
	auto blackRegex = std::regex("black:\\s*(\\d+)", std::regex_constants::icase);
	std::smatch regexMatch;

	Figure* figure = nullptr;

	while (!fin.eof()) {
		std::string buff;
		getline(fin, buff);
		
		if (std::regex_match(buff, regexMatch, whiteRegex)) {
			for (int i = 0; i < std::stoi(regexMatch[1]) && !fin.eof(); i++) {
				getline(fin, buff);
				Position pos(buff);

				figure = new Man(pos, Color::WHITE);
				whiteCheckers.push_back(std::make_shared<Figure*>(figure));
				board[pos.getX()][pos.getY()] = figure;
			}
		}
		else if (std::regex_match(buff, regexMatch, blackRegex)) {
			for (int i = 0; i < std::stoi(regexMatch[1]) && !fin.eof(); i++) {
				getline(fin, buff);
				Position pos(buff);

				figure = new Man(pos, Color::BLACK);
				blackCheckers.push_back(std::make_shared<Figure*>(figure));
				board[pos.getX()][pos.getY()] = figure;
			}
		}
	}

	fin.close();
}

void Checkers::run(Color turnColor) {
	//Moves moves;

	while (true) {

		if (turnColor == Color::WHITE) {
			(*whiteCheckers[0])->possibleMoves(board);
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
