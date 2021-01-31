#ifndef CHESS_GAME_MOVE_HPP
#define CHESS_GAME_MOVE_HPP

#include "defs.hpp"

#define MOVEFLAG_PIECE      0b0000000000000011
#define MOVEFLAG_TYPE       0b0000000000001100
#define MOVEFLAG_START      0b0000001111110000
#define MOVEFLAG_END        0b1111110000000000

#define MOVECASTLE_KS       0b0000000000000000
#define MOVECASTLE_QS       0b0000000000000001

#define MOVETYPE_CASTLE     0b0000000000000100
#define MOVETYPE_ENPASSANT  0b0000000000001000
#define MOVETYPE_PROMOTION  0b0000000000001100

namespace chess_cpp {

/* -------------------------------------------------------------------------- */
/*                             Move bit structure                             */
/* -------------------------------------------------------------------------- */
/**
 * Stored in 16 bits
 * 000000  000000  0000
 * end     start   castling/promotion/en passant flags
 */

class Move {
public:
    U16 value;

    Move(int start, int end, U8 flags);

    U8 get_start();
    U8 get_end();

    std::string to_string();
};

}

#endif // CHESS_GAME_MOVE_HPP
