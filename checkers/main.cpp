#include <iostream>
#include "GameBoard.h"
#include "King.h"
#include "Pawn.h"
using namespace std;

int main() {
	GameBoard board{};
	King k(Position("E5"), Color::WHITE);
	auto p = std::shared_ptr<Piece>(new Pawn(Position("G3"), Color::BLACK));

	board[p->getPosition()] = p;

	cout << "King position: " << k.getPosition() << endl;

	auto moves = k.possibleMoves(board, k.getPosition(), false);
	for (const auto& move : moves) {
		for (const auto& p : move) {
			cout << " -> " << p;
		}
		cout << endl;
	}

	cout << board << endl;

	return 0;
}

void movePiece(
	GameBoard* _board, 
	const Position& from, 
	const Position& to, 
	const Position& previous
) {
	// variable shadowing (just not to write (*_board))
	GameBoard& board = *_board;

	if (!board.onBoard(from) || !board.onBoard(to))
		throw GameBoardException();

	board[to] = board[from];
	board[from] = nullptr;

	board[previous] = nullptr;

	board.addLastMove(to);
}