#include "board/move/move.hpp"

Move::Move(const Move* move) {
    startPos = move->startPos;
    endPos = move->endPos;
    startPieceType = move->startPieceType;
    startPieceColor = move->startPieceColor;
    startPieceFirstMove = move->startPieceFirstMove;
    resultsCheck = move->resultsCheck;
    resultsMate = move->resultsMate;
    isCapture = move->isCapture;
}
Move::Move(const Board* board, int startPos, int endPos) {
    startPos = startPos;
    endPos = endPos;
    Square startSquare = board->getSquare(startPos);
    startPieceColor = startSquare.piece.colour;
    startPieceType = startSquare.piece.pieceType;
    startPieceFirstMove = startSquare.piece.firstMove;
    isCapture = false;
    prevEnPassantPawnVirtual = 0;
}

int Move::getEnPassantPawn(const Board* board) {
    Square endSquare = board->getSquare(this instanceof EnPassant ? ((EnPassant)this).virtualPawnPos : endPos);
    if (endSquare.piece.pieceType.isPawn() &&
        startPieceFirstMove &&
        abs(endPos - startPos) == 16)
        return startPos + (startPieceColor == White ? -8 : 8);
    return -1;
}
bool Move::equals(const Move* move) {
    return startPos == move->startPos && endPos == move->endPos;
}
bool Move::isIn(const std::vector<Move>& moves) {
    for (auto m: moves)
        if (m.equals(this))
            return true;
    return false;
}