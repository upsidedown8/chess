#pragma once
#include "game/move/move.hpp"
#include "game/board.hpp"

class PromotionMove: public Move {
public:
    Square promotionPiece;
    Square endPiece;

    PromotionMove(Board &board, int startPos, int endPos);
    PromotionMove(Board &board, int startPos, int endPos, Square promotionPiece);

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};