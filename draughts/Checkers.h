#pragma once
#include "Figure.h"

#include <vector>
#include <fstream>
#include <string>
#include <regex>

class CheckersException : public std::exception {};

class Checkers {
	Board board{ nullptr };
	std::vector<std::shared_ptr<Figure*>> blackCheckers, whiteCheckers;
public:
	explicit Checkers(const std::string& filename);
	~Checkers() = default;

	void fromFile(const std::string& filename);
};

