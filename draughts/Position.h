#pragma once
#include <iostream>
#include <cstring>

constexpr char BOARD_SIZE = 8;

class PositionException : std::exception {};

class Position {
	char x, y;
public:
	explicit Position(char _x, char _y);
	explicit Position(const std::string&);
	Position(const Position&);
	Position(Position&&);
	~Position() = default;

	std::pair<char, char> get() const;
	char getX() const;
	char getY() const;

	void set(char _x, char _y);
	void setX(char);
	void setY(char);

	bool operator==(const Position&) const;
	Position& operator=(const Position&);

	friend std::ostream& operator<<(std::ostream&, const Position&);
	friend std::istream& operator>>(std::istream&, Position*);
};

