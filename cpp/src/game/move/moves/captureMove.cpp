#include "game/move/moves/captureMove.hpp"

CaptureMove::CaptureMove(Board &board, int start, int end) {
    init(board, start, end);
    endPiece = board[end];
    isCapture = true;
}

void CaptureMove::doMove(Board &board) {
    board.clearSquare(start);
    board[end] = startPiece;
}
void CaptureMove::undoMove(Board &board) {
    board[start] = startPiece;
    board[end] = endPiece;
}