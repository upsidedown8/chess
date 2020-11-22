#if !defined(CHESS_GAME_DEFS_HPP)
#define CHESS_GAME_DEFS_HPP

#include <cstdint>

#define NUM_SQUARES     64

#define WHITE_TO_MOVE   0b00000001
#define WHITE_CASTLE_QS 0b00000010
#define WHITE_CASTLE_KS 0b00000100
#define BLACK_CASTLE_QS 0b00001000
#define BLACK_CASTLE_KS 0b00010000

namespace chess_cpp {

typedef uint64_t      U64;
typedef unsigned char uchar;

enum Pieces {
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
    EMPTY
};

enum Colors {
    WHITE,
    BLACK,
    ALL
};

}

#endif // CHESS_GAME_DEFS_HPP
