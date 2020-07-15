#include "game/move/moves/promotionMove.hpp"

PromotionMove::PromotionMove(Board &board, int startPos, int endPos) {
    init(board, start, end);
    isCapture = board.isOccupied(end);
    endPiece = isCapture ? board[end] : Empty;
    promotionPiece = Board::getPiece(WQueen, Board::isWhite(startPiece));
}

void PromotionMove::doMove(Board &board) {
    board.clearSquare(start);
    board[end] = promotionPiece;
}
void PromotionMove::undoMove(Board &board) {
    board[start] = startPiece;
    board.clearSquare(end);
}