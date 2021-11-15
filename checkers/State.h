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

	explicit State();
	explicit State(
		GameState _gameState,
		Color _turnColor);
	explicit State(const State&);
};
