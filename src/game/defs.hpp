#if !defined(CHESS_GAME_DEFS_HPP)
#define CHESS_GAME_DEFS_HPP

#include <cstdint>
#include <string>

#define NUM_SQUARES     64

namespace chess_cpp {

typedef uint8_t       U8;
typedef uint16_t      U16;
typedef uint64_t      U64;

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, not_on_board
};

const U64 FILES[] {
    0x0101010101010101UL,
    0x0202020202020202UL,
    0x0404040404040404UL,
    0x0808080808080808UL,
    0x1010101010101010UL,
    0x2020202020202020UL,
    0x4040404040404040UL,
    0x8080808080808080UL
};
const U64 RANKS[] {
    0x00000000000000ffUL,
    0x000000000000ff00UL,
    0x0000000000ff0000UL,
    0x00000000ff000000UL,
    0x000000ff00000000UL,
    0x0000ff0000000000UL,
    0x00ff000000000000UL,
    0xff00000000000000UL
};

enum Pieces {
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK,
    NO_PIECE
};

void init();

std::string bb_to_string(U64 bitboard);

U8 calc_pos(int rank, int file);
void calc_rf(U8 pos, int &rank, int &file);

bool is_set(const U64 &board, U8 pos);
void set_pos(U64 &board, U8 pos);
void clr_pos(U64 &board, U8 pos);
U8 count_occupied(U64 board);
U8 pop_lsb(U64 &board);

}

#endif // CHESS_GAME_DEFS_HPP
