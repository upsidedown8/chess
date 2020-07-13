#pragma once
#include "board/piece/piece.hpp"

class Bishop: Piece {
public:
    const int POSSIBLE_MOVES [4]{ -9, -7, 7, 9 };

    Bishop(PieceColor color);
    Bishop(Bishop* bishop);

    virtual Piece* copy();
    virtual std::vector<Move> getPossibleMoves(Board* board);


};