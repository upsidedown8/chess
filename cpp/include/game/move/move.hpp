#pragma once
#include <vector>
#include <math.h>
#include "game/board.hpp"

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

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};