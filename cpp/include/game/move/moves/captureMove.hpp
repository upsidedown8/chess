#include "game/move/move.hpp"

class CaptureMove: public Move {
public:
    CaptureMove();

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};