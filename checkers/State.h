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
	char blackKingN, whiteKingN, white, black;

	explicit State();
	explicit State(
		GameState _gameState,
		Color _turnColor,
		char _whiteKingN = 0,
		char _white = 0,
		char _blackKingN = 0,
		char _black = 0
	);
	explicit State(const State&);
};
