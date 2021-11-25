#include <iostream>
#include "GameBoard.h"
#include "King.h"
#include "Pawn.h"
using namespace std;

void king_movement_test();
void pawn_movement_test();

int main() {
	pawn_movement_test();
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

void king_movement_test() {
	GameBoard board{};
	auto k = std::shared_ptr<Piece>(new King(Position("E5"), Color::WHITE));
	auto p = std::shared_ptr<Piece>(new Pawn(Position("G3"), Color::BLACK));
	auto p2 = std::shared_ptr<Piece>(new Pawn(Position("D6"), Color::BLACK));
	auto p3 = std::shared_ptr<Piece>(new Pawn(Position("B6"), Color::BLACK));
	auto p4 = std::shared_ptr<Piece>(new Pawn(Position("C3"), Color::BLACK));
	auto p5 = std::shared_ptr<Piece>(new Pawn(Position("G5"), Color::BLACK));

	board[p->getPosition()] = p;
	board[p2->getPosition()] = p2;
	board[p3->getPosition()] = p3;
	board[p4->getPosition()] = p4;
	board[p5->getPosition()] = p5;
	board[k->getPosition()] = k;

	cout << "King position: " << k->getPosition() << endl;

	auto moves = k->possibleMoves(board, k->getPosition(), false);
	for (const auto& move : moves) {
		int cnt = 0;
		for (auto i = move.size() - 1; i >= 0; i--) {
			if (cnt++ != 0)
				cout << " -> ";
			cout << move[i];

			if (i == 0) break;
		}
		cout << endl;
	}

	cout << board << endl;
}

void pawn_movement_test() {
	GameBoard board{};
	auto k = std::shared_ptr<Piece>(new Pawn(Position("E5"), Color::WHITE));
	auto p = std::shared_ptr<Piece>(new Pawn(Position("G3"), Color::BLACK));
	auto p2 = std::shared_ptr<Piece>(new Pawn(Position("D6"), Color::BLACK));
	auto p3 = std::shared_ptr<Piece>(new Pawn(Position("B6"), Color::BLACK));
	auto p4 = std::shared_ptr<Piece>(new Pawn(Position("B4"), Color::BLACK));
	auto p5 = std::shared_ptr<Piece>(new Pawn(Position("D4"), Color::BLACK));
	auto p6 = std::shared_ptr<Piece>(new Pawn(Position("F6"), Color::BLACK));

	board[p->getPosition()] = p;
	board[p2->getPosition()] = p2;
	board[p3->getPosition()] = p3;
	board[p4->getPosition()] = p4;
	board[p5->getPosition()] = p5;
	board[p6->getPosition()] = p6;
	board[k->getPosition()] = k;

	cout << "King position: " << k->getPosition() << endl;

	auto moves = k->possibleMoves(board, k->getPosition(), false);
	for (const auto& move : moves) {
		int cnt = 0;
		for (auto i = move.size() - 1; i >= 0; i--) {
			if (cnt++ != 0)
				cout << " -> ";
			cout << move[i];

			if (i == 0) break;
		}
		cout << endl;
	}

	cout << board << endl;
}
