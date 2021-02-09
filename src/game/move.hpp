#ifndef CHESS_GAME_MOVE_HPP
#define CHESS_GAME_MOVE_HPP

#include "defs.hpp"

#define MOVEPROMOTION_PAWN   0b0000000000000000
#define MOVEPROMOTION_KNIGHT 0b0000000000000001
#define MOVEPROMOTION_BISHOP 0b0000000000000010
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

    Move();
    Move(int start, int end, U8 flags);

    U8 get_start();
    U8 get_end();

    std::string to_string();
};

class UndoInfo {
private:
    U8 castling;
    U8 fifty_move;
    U8 en_passant;
    int captured;
    
public:
    UndoInfo(int castling, int fiftyMove, int enPassant, int piece);
    
    U8 get_castling();
    U8 get_fifty_move();
    U8 get_en_passant();
    int get_captured_piece();
};

}

#endif // CHESS_GAME_MOVE_HPP
