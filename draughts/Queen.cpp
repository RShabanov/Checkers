#include "Queen.h"

Queen::Queen(const Position& positon, Color color)
	: Figure(positon, color) {}

Moves Queen::possibleMoves(const Board& board) const {
	Moves moves;

	//for (int x = -1; x < 2; x += 2) {
	//	for (int y = -1; y < 2; y += 2) {
	//		int nx = position.getX() + x,
	//			ny = position.getY() + y;

	//		if (nx < 0 || nx >= BOARD_SIZE ||
	//			ny < 0 || ny >= BOARD_SIZE)
	//			continue;

	//		// if the target cell is not empty
	//		if (board[nx][ny] != nullptr) {
	//			if (board[nx][ny]->getColor() != color) {
	//				// if cells after the target cell is empty
	//				nx += x;
	//				ny += y;
	//			}
	//			else continue;
	//		}


	//		while (board[nx][ny] == nullptr && 
	//			(nx >= 0 && nx < BOARD_SIZE || 
	//			ny >= 0 && ny < BOARD_SIZE)) {

	//			moves.emplace_back(Position(nx, ny));
	//			nx += x;
	//			ny += y;
	//		}
	//	}
	//}

	return moves;
}

bool Queen::isQueen() const { return true; }
