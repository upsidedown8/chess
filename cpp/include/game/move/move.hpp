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
public:
    Square startPiece;
    bool isCapture;
    int start, end;

    bool operator==(Move &other);

    virtual void doMove(const Board &board);
    virtual void undoMove(const Board &board);
};