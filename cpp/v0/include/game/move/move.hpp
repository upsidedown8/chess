#pragma once
#include "game/board.hpp"
#include <vector>
#include <math.h>

enum MoveInfo {
    Success,
    IllegalMove,
    TryToTakeFriendlyPiece,
    TryToMoveEmptyPiece,
    TryToMoveEmptySquare,
    TryToMoveSquareNotOnBoard,
    TryToRedoBeforeMoveMade,
    TryToUndoBeforeMoveMade
};

class Move {
protected:
    void init(Board &board, int start, int end);
public:
    Square startPiece;
    bool isCapture;
    int start, end;

    bool operator==(Move &other);

    virtual void doMove(Board &board) = 0;
    virtual void undoMove(Board &board) = 0;
};
