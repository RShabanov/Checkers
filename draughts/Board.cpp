#include "Board.h"

bool Board::isEmpty(char x, char y) const {
	return data[x][y] == nullptr;
}

double Board::score() const {
	return black.size() - white.size();
}

Figure* Board::operator[](const Position& position) {
	if (position.getX() < 0 || position.getX() >= BOARD_SIZE ||
		position.getY() < 0 || position.getY() >= BOARD_SIZE)
		throw BoardException();

	return data[position.getX()][position.getY()];
}

void Board::changeTurn() {
	state.turnColor = state.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;
}
