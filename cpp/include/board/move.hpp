#pragma once
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
public:
    PieceColor startPieceColor;
    PieceType startPieceType;
    bool isCapture, startPieceFirstMove;
    bool resultsCheck, resultsMate;
    int startPos, endPos;
    int prevDraw50, prevEnPassantPawnVirtual;

    Move(const Move* move);
    Move(const Board* board, int startPos, int endPos);

    int getEnPassantPawn(const Board* board);
    bool equals(const Move* move);
    bool isIn(const std::vector<Move>& moves);

    virtual Move copy() = 0;
    virtual void doMove(const Board* board);
    virtual void undoMove(const Board* board);
};