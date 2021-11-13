#pragma once
#include "Figure.h"
#include "Color.h"
#include "Man.h"
#include "Queen.h"
#include "Board.h"

#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <tuple>
#include <limits>

class CheckersException : public std::exception {};

class Checkers {
	Board board;
	Moves history;

	std::vector<Board> getAllMoves(const Board& board, Color color);
	void simulateMove(Board* board, Position position, const std::vector<Position>& move);

	std::pair<double, Board> minimax(Board& board, int depth, bool max);

	Board createBoard(const Board& board) const;

public:
	explicit Checkers(const std::string& filename);
	~Checkers() = default;

	void fromFile(const std::string& filename);

	void run(Color turnColor = Color::WHITE);
	void runNMoves(unsigned int moveNumber, Color turnColor = Color::WHITE);

	void save(const std::string& filename = "checkers-game.txt") const;

	GameState state() const;
};

