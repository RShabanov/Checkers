#include "Figure.h"

Figure::Figure(const Position& _positon, Color _color)
// поскольку нет конструктора по умолчанию для класса Position
// необходимо написать так:
	: position(_positon), color(_color) {}

Color Figure::getColor() const { return color; }

const Position& Figure::getPosition() const {
	return position;
}

void Figure::setPosition(char x, char y) {
	position.set(x, y);
}

void Figure::setPosition(const Position& pos) {
	position = pos;
}

char Figure::getX() const {
	return position.getX();
}

char Figure::getY() const {
	return position.getY();
}
