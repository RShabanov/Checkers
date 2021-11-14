#include "State.h"

State::State() : State(GameState::STILL_PLAYING, Color::WHITE) {}

State::State(
	GameState _gameState,
	Color _turnColor,
	char _whiteQueenN,
	char _white,
	char _blackQueenN,
	char _black)
	: gameState(_gameState), turnColor(_turnColor),
	whiteQueenN(_whiteQueenN), white(_white),
	blackQueenN(_blackQueenN), black(_black) {}
