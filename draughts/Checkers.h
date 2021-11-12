#pragma once
#include "Figure.h"
#include "Color.h"
#include "Man.h"
#include "Queen.h"

#include <vector>
#include <fstream>
#include <string>
#include <regex>

class CheckersException : public std::exception {};

class Checkers {
	Board board{ nullptr };
	std::vector<std::shared_ptr<Figure>> blackCheckers, whiteCheckers;
	Moves moves;
	Color turnColor;

	void move(Position, std::vector<Position>&);
	int score() const;

public:
	explicit Checkers(const std::string& filename);
	~Checkers();

	void fromFile(const std::string& filename);

	void run(Color turnColor = Color::WHITE);

	void save(const std::string& filename = "checkers-game.txt") const;
};

