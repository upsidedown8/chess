#include "piece.hpp"

Piece::Piece(bool firstMove, PieceColor color) {
    this->color = color;
    this->firstMove = firstMove;
}
PieceType Piece::getPieceTypeByNotation(char notation) {
    switch (tolower(notation)) {
        case 'p': return Pawn;
        case 'n': return Knight;
        case 'b': return Bishop;
        case 'r': return Rook;
        case 'q': return Queen;
        case 'k': return King;
        default:  return Empty;
    }
}
Piece Piece::getPieceByPieceType(PieceType pieceType, PieceColor color) {
    switch(pieceType){
        case Pawn:   return new Pawn(color);
        case Bishop: return new Bishop(color);
        case Knight: return new Knight(color);
        case Rook:   return new Rook(color);
        case Queen:  return new Queen(color);
        case King:   return new King(color);
        default:     return NULL;
    }
}