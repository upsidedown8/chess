#pragma once;
#include "board/move/move.hpp"
#include <vector>
#include <iostream>

enum PieceType {
    Pawn   = 0,
    Knight = 1,
    Bishop = 2,
    Rook   = 3,
    Queen  = 4,
    King   = 5,
    Empty  = 6
};

enum PieceColor {
    White, Black
};

class Piece {
public:
    bool firstMove;
    PieceColor color;
    PieceType pieceType;

    Piece(bool firstMove, PieceColor pieceColor);
    static PieceType getPieceTypeByNotation(char notation);
    static Piece* getPieceByPieceType(PieceType pieceType, int pos, PieceColor pieceColor);

    virtual Piece* copy() = 0;
    virtual std::vector<Move> getPossibleMoves(Board* board) = 0;
};
