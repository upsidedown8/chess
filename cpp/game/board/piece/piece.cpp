#include "piece.hpp"

Piece::Piece(int pos, bool firstMove, PieceColor color) {
    this->pos = pos;
    this->color = color;
    this->firstMove = firstMove;
}
PieceType Piece::getPieceTypeByNotation(char notation) {
    for (PieceType pieceType : PieceType.values())
        if (pieceType.getCode() == toUpperCase(notation))
            return pieceType;
    return null;
}
Piece Piece::getPieceByPieceType(PieceType pieceType, int pos, PieceColor color) {
    switch(pieceType){
        case Pawn:   return new Pawn(pos, color);
        case Bishop: return new Bishop(pos, color);
        case Knight: return new Knight(pos, color);
        case Rook:   return new Rook(pos, color);
        case Queen:  return new Queen(pos, color);
        case King:   return new King(pos, color);
        default:     return NULL;
    }
}