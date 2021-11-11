#pragma once
#include "Figure.h"
#include "Color.h"

#include <vector>
#include <fstream>
#include <string>
#include <regex>

class CheckersException : public std::exception {};

class Checkers {
	Board board{ nullptr };
	std::vector<std::shared_ptr<Figure*>> blackCheckers, whiteCheckers;
	std::vector<std::pair<Position, Position>> moves;

public:
	explicit Checkers(const std::string& filename);
	~Checkers() = default;

	void fromFile(const std::string& filename);

	void run(Color turnColor = Color::WHITE);

	void save(const std::string& filename = "checkers-game.txt") const;
};

