#pragma once
#include "Color.h"

enum class GameState : char {
	BLACK_WON,
	WHITE_WON,
	STILL_PLAYING,
};

struct State {
	Color turnColor;
	GameState gameState;
	char blackQueenN, whiteQueenN, white, black;

	explicit State();
	explicit State(
		GameState _gameState,
		Color _turnColor,
		char _whiteQueenN = 0,
		char _white = 0,
		char _blackQueenN = 0,
		char _black = 0
	);
};
