#include "GameBoard.h"

GameBoard::GameBoard(const State& _state, const Board& _board, const Move& _lastMove)
	: state(_state), lastMove(_lastMove), board(_board) {}

GameBoard::GameBoard(const GameBoard& gameBoard) 
	: state(gameBoard.state), board(gameBoard.board), lastMove(gameBoard.lastMove) {}

GameBoard::GameBoard(GameBoard&& gameBoard)
	: state(gameBoard.state), board(gameBoard.board), lastMove(gameBoard.lastMove) {}

bool GameBoard::isEmpty(char y, char x) const {
	if (!onBoard(y, x))
		throw GameBoardException();
	return board[y][x] == nullptr;
}

bool GameBoard::isEmpty(const Position& position) const {
	return isEmpty(position.getY(), position.getX());
}

bool GameBoard::onBoard(char y, char x) const {
	return !(y < 0 || y >= BOARD_SIZE ||
			x < 0 || x >= BOARD_SIZE);
}

bool GameBoard::onBoard(const Position& position) const {
	return onBoard(position.getY(), position.getX());
}

void GameBoard::changeTurn() {
	state.turnColor = state.turnColor == Color::WHITE ? Color::BLACK : Color::WHITE;
}

void GameBoard::changeGameState(GameState gameState) {
	state.gameState = gameState;
}

void GameBoard::increaseKing(Color _color) {
	if (_color == Color::WHITE)
		state.whiteKingN++;
	else state.blackKingN++;
}

double GameBoard::score() const {
	return double(state.white) - double(state.black) + double(state.blackKingN) * 0.5 - double(state.whiteKingN) * 0.5;
}

char GameBoard::getBlackKingN() const {
	return state.blackKingN;
}

char GameBoard::getWhiteKingN() const {
	return state.whiteKingN;
}

Piece*& GameBoard::operator[](const Position& position) {
	return (*this)[position.getY()][position.getX()];
}

const Piece* const& GameBoard::operator[](const Position& position) const {
	return (*this)[position.getY()][position.getX()];
}

GameBoard::BoardRow& GameBoard::operator[](char y) {
	if (onBoard(y, 0)) return board[y];

	// otherwise method "onBoard" throws exception
	// to remove warnings
	throw GameBoardException();
}

const GameBoard::BoardRow& GameBoard::operator[](char y) const {
	if (onBoard(y, 0)) return board[y];

	// otherwise method "onBoard" throws exception
	// to remove warnings
	throw GameBoardException();
}

void GameBoard::printHistory(std::ostream& out) const {
	out << (state.turnColor == Color::WHITE ? " White: " : " Black: ");
	for (size_t i = 0; i < lastMove.size() - 1; i++)
		out << lastMove[i] << " -> ";
	out << lastMove.back() << std::endl;
}

GameBoard&& GameBoard::copy() const {
	return std::move(GameBoard(state, board, lastMove));
}

std::ostream& operator<<(std::ostream& out, const GameBoard& gameBoard) {
	out.width(4);
	out.fill(' ');
	out << '+';

	for (int i = 0; i < BOARD_SIZE; i++) {
		out.width(4);
		out.fill('-');
		out << '+';
	}
	out << std::endl;

	for (int x = 0; x < BOARD_SIZE; x++) {
		out << " " << BOARD_SIZE - x << " | ";
		for (int y = 0; y < BOARD_SIZE; y++) {
			char s = ' ';

			if (!gameBoard.isEmpty(y, BOARD_SIZE - 1 - x)) {
				// direct access to object field not to use extra checks
				s = pieceColor(gameBoard.board[y][x]) == Color::WHITE ? 'w' : 'b';

				// since GameBoard DOES NOT see King class
				if (isKing(gameBoard.board[y][x]))
					s = std::toupper(s);
			}

			out << s << " | ";
		}
		out << std::endl;

		out.width(4);
		out.fill(' ');
		out << '+';

		for (int i = 0; i < BOARD_SIZE; i++) {
			out.width(4);
			out.fill('-');
			out << '+';
		}
		out << std::endl;
	}

	out << "  ";
	for (int i = 0; i < BOARD_SIZE; i++) {
		out.width(3);
		out.fill(' ');
		out << " " << char('A' + i);
	}
	out << std::endl;

	return out;
}
