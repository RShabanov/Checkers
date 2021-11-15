#include "Piece.h"

Piece::Piece(const Position& _positon, Color _color)
	: position(_positon), color(_color) {}

bool Piece::isWhite() const {
	return color == Color::WHITE;
}

bool Piece::isBlack() const {
	return !isWhite();
}

Color Piece::getColor() const { return color; }

const Position& Piece::getPosition() const {
	return position;
}

char Piece::getX() const {
	return position.getX();
}

char Piece::getY() const {
	return position.getY();
}

void Piece::setPosition(char x, char y) {
	position.set(x, y);
}

void Piece::setPosition(const Position& pos) {
	position = pos;
}

void Piece::setX(char _x) {
	position.setX(_x);
}

void Piece::setY(char _y) {
	position.setY(_y);
}

Color pieceColor(const Piece* const piece) {
	return piece->getColor();
}

bool isKing(const Piece* const piece) {
	return piece->isKing();
}
