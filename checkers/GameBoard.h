#pragma once
#include "Position.h"
#include "Pieces.h"
#include "State.h"

#include <array>
#include <vector>
#include <exception>

constexpr int BOARD_SIZE = 8;

class GameBoardException : public std::exception {};

class GameBoard {
public:
	using BoardRow = std::array<Piece*, BOARD_SIZE>;
	using Board = std::array<BoardRow, BOARD_SIZE>;

	explicit GameBoard(
		const State& _state = State(),
		const Pieces& _pieces = Pieces(),
		const Move& _lastMove = {});
	explicit GameBoard(const GameBoard&);
	explicit GameBoard(GameBoard&&);
	~GameBoard() = default;

	bool isEmpty(char y, char x) const;
	bool isEmpty(const Position&) const;

	bool onBoard(char y, char x) const;
	bool onBoard(const Position&) const;

	void changeTurn();
	void changeGameState(GameState);

	void increaseKing(Color);

	double score() const;

	char getBlackKingN() const;
	char getWhiteKingN() const;

	Piece*& operator[](const Position&);
	const Piece* const& operator[](const Position&) const;
	BoardRow& operator[](char y);
	const BoardRow& operator[](char y) const;

	friend std::ostream& operator<<(std::ostream&, const GameBoard&);
	void printHistory(std::ostream&) const;

private:
	Board board;
	Pieces pieces;

	State state;
	Move lastMove;
};

