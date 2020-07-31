#pragma once
#include "game/move/move.hpp"
#include "game/board.hpp"

class CastleMove: public Move {
public:
    Square rookPiece;
    bool isKingSide;
    int rookStart, rookEnd;
    CastleMove(Board &board, int start, int end);
    
    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};