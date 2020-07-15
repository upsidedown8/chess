#include "game/move/move.hpp"

class PromotionMove: public Move {
public:
    Square promotionPiece;
    Square endPiece;

    PromotionMove(Board &board, int startPos, int endPos);

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};