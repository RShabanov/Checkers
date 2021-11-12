#pragma once
#include <array>
#include <exception>

#include "Figure.h"

enum class GameState : char {
	BLACK_WON,
	WHITE_WON,
	STILL_PLAYING,
};

struct State {
	Color turnColor;
	GameState state;
};

class BoardException : std::exception {};

struct Board {
	std::array<std::array<Figure*, BOARD_SIZE>, BOARD_SIZE> data{ nullptr };
	std::vector<std::shared_ptr<Figure>> black, white;
	State state;

	bool isEmpty(char x, char y) const;
	double score() const;

	Figure* operator[](const Position&);

	void changeTurn();
	void changeGameState(GameState);
};