#ifndef CHESS_GAME_MOVE_HPP
#define CHESS_GAME_MOVE_HPP

#include "defs.hpp"

#define MOVEPROMOTION_FLAG  0b0000000000000011
#define MOVETYPE_FLAG       0b0000000000001100
#define MOVESTART_FLAG      0b0000001111110000
#define MOVEEND_FLAG        0b1111110000000000

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
