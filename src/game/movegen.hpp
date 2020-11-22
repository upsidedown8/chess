#if !defined(CHESS_GAME_MOVEGEN_HPP)
#define CHESS_GAME_MOVEGEN_HPP

#include "board.hpp"
#include <vector>



enum MoveType {
    SimpleMove,

};

struct Move {
    uchar start, end;
};



#endif // CHESS_GAME_MOVEGEN_HPP
