#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

class PositionException : std::exception {};

// Position class DOES NOT check if it is valid position
class Position {
	char y, x;
public:
	explicit Position(char _y, char _x);
	explicit Position(const std::string&);
	Position(const Position&);
	Position(Position&&);
	~Position() = default;

	char getX() const;
	char getY() const;

	void set(char _y, char _x);
	void set(const std::string&);
	void setX(char);
	void setY(char);

	void add(char _y, char _x);
	void addX(char);
	void addY(char);

	bool operator==(const Position&) const;
	Position& operator=(const Position&);

	friend std::ostream& operator<<(std::ostream&, const Position&);
	friend std::istream& operator>>(std::istream&, Position&);
};

using Move = std::vector<Position>;
using Moves = std::vector<Move>;
