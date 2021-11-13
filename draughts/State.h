#pragma once
#include "Color.h"

enum class GameState : char {
	BLACK_WON,
	WHITE_WON,
	STILL_PLAYING,
};

struct State {
	Color turnColor;
	GameState state;
};

