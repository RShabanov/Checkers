#include "State.h"

State::State() : State(GameState::STILL_PLAYING, Color::WHITE) {}

State::State(
	GameState _gameState,
	Color _turnColor)
	: gameState(_gameState), turnColor(_turnColor) {}

State::State(const State& state)
	: gameState(state.gameState), turnColor(state.turnColor) {}
