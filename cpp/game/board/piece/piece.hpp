#pragma once;
#include "game/board/move/move.hpp"
#include <vector>

enum PieceType {
    Pawn, Knight, Bishop, Rook, Queen, King, Empty
};

enum PieceColor {
    White, Black
};

class Piece {
public:
    int pos;
    bool firstMove;
    PieceColor color;
    PieceType pieceType;

    Piece(int pos, bool firstMove, PieceColor pieceColor);
    static PieceType getPieceTypeByNotation(char notation);
    static Piece* getPieceByPieceType(PieceType pieceType, int pos, PieceColor pieceColor);

    virtual Piece* copy() = 0;
    virtual std::vector<Move> getPossibleMoves(Board* board) = 0;
};
