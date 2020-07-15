#include "game/move/move.hpp"

class EnPassantMove: public Move {
public:
    EnPassantMove();

    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};