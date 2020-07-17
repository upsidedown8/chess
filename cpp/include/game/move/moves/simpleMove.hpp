#include "game/move/move.hpp"
#include "game/board.hpp"

class SimpleMove: public Move {
public:
    SimpleMove(Board &board, int start, int end);
    
    virtual void doMove(Board &board);
    virtual void undoMove(Board &board);
};
