#include "Figure.h"

Figure::Figure(const Position& _positon, Color _color)
	: position(_positon), color(_color) {}

bool Figure::isWhite() const {
	return color == Color::WHITE;
}

bool Figure::isBlack() const {
	return !isWhite();
}

Color Figure::getColor() const { return color; }

const Position& Figure::getPosition() const {
	return position;
}

char Figure::getX() const {
	return position.getX();
}

char Figure::getY() const {
	return position.getY();
}

void Figure::setPosition(char x, char y) {
	position.set(x, y);
}

void Figure::setPosition(const Position& pos) {
	position = pos;
}

void Figure::setX(char _x) {
	position.setX(_x);
}

void Figure::setY(char _y) {
	position.setY(_y);
}