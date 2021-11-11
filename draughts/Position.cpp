#include "Position.h"

Position::Position(char _x, char _y)
	: x(_x % BOARD_SIZE), y(_y % BOARD_SIZE) {}

Position::Position(const std::string& pos) {
	if (pos.size() != 2) throw PositionException();
	x = toupper(pos.front());
	if (x < 'A' || x > 'H')
		throw PositionException();
	x -='A';

	y = pos.back();
	if (y < '1' || y > '8')
		throw PositionException();
	y = BOARD_SIZE - 1 - (y - '1');
}

Position::Position(const Position& pos)
	: x(pos.x), y(pos.y) {}

Position::Position(Position&& pos)
	: x(pos.x), y(pos.y) {}

std::pair<char, char> Position::get() const {
	return std::pair<char, char>(x, y);
}

char Position::getX() const { return x; }

char Position::getY() const { return y; }

void Position::set(char _x, char _y) {
	setX(_x);
	setY(_y);
}

void Position::setX(char _x) {
	if (_x < 0 || _x > 7)
		throw PositionException();
	x = _x;
}

void Position::setY(char _y) {
	if (_y < 0 || _y > 7)
		throw PositionException();
	y = _y;
}

bool Position::operator==(const Position& o) const {
	return x == o.x && y == o.y;
}

Position& Position::operator=(const Position& o) {
	x = o.x;
	y = o.y;
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << ('A' + pos.x) << pos.y + 1;
	return out;
}

std::istream& operator>>(std::istream& in, Position* pos) {
	in >> pos->x;
	pos->x = toupper(pos->x);
	if (pos->x < 'A' || pos->x > 'H')
		throw PositionException();
	pos->x %= 'A';

	in >> pos->y;
	if (pos->y < '1' || pos->y > '8')
		throw PositionException();
	pos->y = BOARD_SIZE - 1 - (pos->y - '1');

	return in;
}
