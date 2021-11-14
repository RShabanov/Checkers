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

// to check checkers placement
// so there were no moments like
// one figure is on A1 and another is on A2
enum class CheckersPlacement : char {
	UNDEFINED,
	EVEN,
	ODD
};

class Checkers {
	Board board;
	Moves history;
	Color firstMove;

	std::vector<Board> getAllMoves(const Board& board, Color color);
	void simulateMove(Board* board, Position position, const std::vector<Position>& move);

	std::pair<double, Board> minimax(Board& board, int depth, bool whiteTurn);
	std::pair<double, Board> alphaBeta(Board& board, int depth, double aplha, double beta, bool whiteTurn);

public:
	explicit Checkers(const std::string& filename);
	~Checkers() = default;

	void fromFile(const std::string& filename);

	void run(Color turnColor = Color::WHITE);
	void runNMoves(unsigned int moveNumber, Color turnColor = Color::WHITE);

	void save(const std::string& filename = "checkers-game.txt") const;

	GameState state() const;
};

