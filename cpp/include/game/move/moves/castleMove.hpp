#include "game/move/move.hpp"

class CastleMove: public Move {
public:
    CastleMove();

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};