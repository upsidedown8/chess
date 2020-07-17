#pragma once
#include "game/move/move.hpp"
#include "game/board.hpp"

class CaptureMove: public Move {
public:
    Square endPiece;
    CaptureMove(Board &board, int start, int end);

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};