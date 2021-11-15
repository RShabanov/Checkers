#include "State.h"

State::State() : State(GameState::STILL_PLAYING, Color::WHITE) {}

State::State(
	GameState _gameState,
	Color _turnColor,
	char _whiteKingN,
	char _white,
	char _blackKingN,
	char _black)
	: gameState(_gameState), turnColor(_turnColor),
	whiteKingN(_whiteKingN), white(_white),
	blackKingN(_blackKingN), black(_black) {}

State::State(const State& state)
	: gameState(state.gameState), turnColor(state.turnColor),
	white(state.white), whiteKingN(state.whiteKingN),
	black(state.black), blackKingN(state.blackKingN) {}
