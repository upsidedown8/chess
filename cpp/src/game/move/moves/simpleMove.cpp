#include "game/move/moves/simpleMove.hpp"

SimpleMove::SimpleMove(Board &board, int start, int end) {
    init(board, start, end);
    isCapture = false;
}

void SimpleMove::doMove(Board &board) {
    board.clearSquare(start);
    board[end] = startPiece;
}
void SimpleMove::undoMove(Board &board) {
    board[start] = startPiece;
    board.clearSquare(end);
}