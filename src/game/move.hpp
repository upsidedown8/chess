#ifndef CHESS_GAME_MOVE_HPP
#define CHESS_GAME_MOVE_HPP

#include "defs.hpp"

namespace chess_cpp {

/* -------------------------------------------------------------------------- */
/*                             Move bit structure                             */
/* -------------------------------------------------------------------------- */
/**
 * Stored in 16 bits
 * 000000 000000 00              00
 * end    start  promotion piece castling/promotion/en passant flag
 */

class Move {
public:
    U8 start, end;

    U8 get_start();
    U8 get_end();

    Move(U8 start, U8 end);
};

}

#endif // CHESS_GAME_MOVE_HPP
