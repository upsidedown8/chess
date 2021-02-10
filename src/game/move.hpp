#ifndef CHESS_GAME_MOVE_HPP
#define CHESS_GAME_MOVE_HPP

#include "defs.hpp"

#define MOVEPROMOTION_KNIGHT 0b0000000000000000
#define MOVEPROMOTION_BISHOP 0b0000000000000001
#define MOVEPROMOTION_ROOK   0b0000000000000010
#define MOVEPROMOTION_QUEEN  0b0000000000000011

#define MOVEFLAG_PIECE       0b0000000000000011
#define MOVEFLAG_TYPE        0b0000000000001100
#define MOVEFLAG_START       0b0000001111110000
#define MOVEFLAG_END         0b1111110000000000

#define MOVECASTLE_KS        0b0000000000000000
#define MOVECASTLE_QS        0b0000000000000001

#define MOVETYPE_CASTLE      0b0000000000000100
#define MOVETYPE_ENPASSANT   0b0000000000001000
#define MOVETYPE_PROMOTION   0b0000000000001100

#define CREATE_MOVE(start, end, flags) \
    (end<<10) | (start<<4) | (flags)

#define GET_MOVE_START(move, start) \
    start = (move & MOVEFLAG_START)>>4;

#define GET_MOVE_END(move, end) \
    end = (move & MOVEFLAG_END)>>10;

#define GET_MOVE_TYPE(move, type) \
    type = move & MOVEFLAG_TYPE;

#define GET_MOVE_PIECE(move, piece) \
    piece = move & MOVEFLAG_PIECE;

namespace chess_cpp {

/* -------------------------------------------------------------------------- */
/*                             Move bit structure                             */
/* -------------------------------------------------------------------------- */
/**
 * Stored in 16 bits
 * 000000  000000  0000
 * end     start   castling/promotion/en passant flags
 */

typedef U16 Move;

std::string move_to_string(Move &move);

struct UndoInfo {
    U8 castling;
    U8 fifty_move;
    U8 en_passant;
    int captured;
};

}

#endif // CHESS_GAME_MOVE_HPP
