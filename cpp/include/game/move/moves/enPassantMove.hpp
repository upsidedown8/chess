#pragma once
#include "game/move/move.hpp"
#include "game/board.hpp"

class EnPassantMove: public Move {
public:
    int enemyPawnPos;
    Square endPiece;
    EnPassantMove(Board &board, int start, int end);

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};