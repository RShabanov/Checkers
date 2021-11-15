#include <iostream>
#include "GameBoard.h"
using namespace std;

int main() {
	/*GameBoard board{ (State()) };
	
	cout << board << endl;*/

	return 0;
}

GameBoard&& copyBoard(const GameBoard&) {
	return std::move(GameBoard());
}
void movePiece(GameBoard*, const Position& from, const Position& to) {

}