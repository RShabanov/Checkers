#include "King.h"

King::King(const Position& _position, Color _color)
    : Piece(_position, _color) {}

Moves King::possibleMoves(const GameBoard& board, const Position& from, bool onlyAttack) const {
    Moves killMoves, usualMoves;
    
    // checks neighbourhood:
    // (-n, -n) . (-n, n)
    //	   .	o	 .
    // ( n,  n) . ( n, n)
    for (int y = -1; y < 2; y += 2) {
        for (int x = -1; x < 2; x += 2) {
            int nx = from.getX() + x,
                ny = from.getY() + y;

            Position to(ny, nx);

            while (board.onBoard(to)) {
                Position previous(to);

                if (!board.isEmpty(to)) {
                    if (board[to]->getColor() == color) break;

                    while (true) {
                        previous = to;
                        to.add(y, x);

                        if (board.onBoard(to) && board.isEmpty(to)) {
                            // since we can't affect on board
                            GameBoard newBoard(board.copy());

                            movePiece(&newBoard, from, to, previous);
                            auto moves = newBoard[to]->possibleMoves(newBoard, to, true);

                            if (moves.empty())
                                killMoves.emplace_back(Move(1, to));

                            for (const auto& move : moves) {
                                killMoves.emplace_back(Move(1, to));
                                killMoves.back().insert(killMoves.back().begin(),
                                    move.begin(), move.end());
                            }
                        }
                        else break;
                    }
                    break;
                }
                else if (!onlyAttack && killMoves.empty())
                    usualMoves.emplace_back(Move(1, to));

                previous = to;
                to.add(y, x);
            }
        }
    }

    return killMoves.empty() ? std::move(usualMoves) : std::move(killMoves);
}

bool King::isKing() const { return true; }
