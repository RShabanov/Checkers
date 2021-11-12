#include "Figure.h"

Figure::Figure(const Position& _positon, Color _color)
	: position(_positon), color(_color) {}

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
