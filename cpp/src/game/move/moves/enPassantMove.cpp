#include "game/move/moves/enPassantMove.hpp"

EnPassantMove::EnPassantMove(Board &board, int start, int end) {
    init(board, start, end);
    enemyPawnPos = end + (Board::isWhite(startPiece) ? 8 : -8);
    endPiece = board[enemyPawnPos];
    isCapture = true;
}

void EnPassantMove::doMove(Board &board) {
    board.clearSquare(start);
    board.clearSquare(enemyPawnPos);
    board[end] = startPiece;
}
void EnPassantMove::undoMove(Board &board) {
    board[start] = startPiece;
    board[enemyPawnPos] = endPiece;
    board.clearSquare(end);
}