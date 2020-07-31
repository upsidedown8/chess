#include "game/move/moves/castleMove.hpp"

CastleMove::CastleMove(Board &board, int start, int end) {
    init(board, start, end);
    isKingSide = end - start == 2;
    rookStart = (isKingSide ? 7 : 0) + (Board::isWhite(startPiece) ? 56 : 0);
    rookEnd = rookStart + (isKingSide ? -2 : 3);
    rookPiece = Board::getPiece(WRook, Board::isWhite(startPiece));
}

void CastleMove::doMove(Board &board) {
    board.clearSquare(start);
    board[end] = startPiece;

    board.clearSquare(rookStart);
    board[rookEnd] = rookPiece;
}
void CastleMove::undoMove(Board &board) {
    board[start] = startPiece;
    board.clearSquare(end);

    board[rookStart] = rookPiece;
    board.clearSquare(rookEnd);
}