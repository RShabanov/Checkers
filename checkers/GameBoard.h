#pragma once
#include "Position.h"
#include "PiecesInfo.h"
#include "State.h"

#include <array>
#include <vector>
#include <exception>

constexpr int BOARD_SIZE = 8;

class GameBoardException : public std::exception {};

class GameBoard {
public:
	using BoardRow = std::array<std::shared_ptr<Piece>, BOARD_SIZE>;
	using Board = std::array<BoardRow, BOARD_SIZE>;

	explicit GameBoard(
		const State& _state = State(),
		const Board& _board = { nullptr },
		const PiecesInfo& _pieces = PiecesInfo(),
		const Move& _lastMove = {});
	explicit GameBoard(GameBoard&&);
	~GameBoard() = default;

	bool isEmpty(char y, char x) const;
	bool isEmpty(const Position&) const;

	bool onBoard(char y, char x) const;
	bool onBoard(const Position&) const;

	void changeTurn();
	void changeGameState(GameState);

	double score() const;

	std::shared_ptr<Piece>& operator[](const Position&);
	const std::shared_ptr<Piece>& operator[](const Position&) const;
	BoardRow& operator[](char y);
	const BoardRow& operator[](char y) const;

	friend std::ostream& operator<<(std::ostream&, const GameBoard&);
	void printHistory(std::ostream&) const;

	PiecesInfo piecesInfo;

private:
	Board board;
	State state;
	Move lastMove;
};

