#ifndef CHESS_GAME_DEFS_HPP
#define CHESS_GAME_DEFS_HPP

#include <cstdint>
#include <string>

#define NUM_SQUARES 64

#define FULL_BB  0xffffffffffffffffULL

namespace chess_cpp {

typedef uint8_t     U8;
typedef uint16_t    U16;
typedef uint64_t    U64;

/* -------------------------------------------------------------------------- */
/*                                   Squares                                  */
/* -------------------------------------------------------------------------- */
// bitboard mapping
enum Squares {
    // LSB (0) = a8
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    // MSB (63) = h1
    not_on_board
};

/* -------------------------------------------------------------------------- */
/*                                   Tables                                   */
/* -------------------------------------------------------------------------- */
extern U64 RANKS[256];
extern U64 FILES[256];
extern U64 NOT_RANKS[256];
extern U64 NOT_FILES[256];

extern U64 SLIDER_RANGE[NUM_SQUARES][NUM_SQUARES];

extern U64 SET_BIT_TABLE[NUM_SQUARES];
extern U64 CLEAR_BIT_TABLE[NUM_SQUARES];

/* -------------------------------------------------------------------------- */
/*                                Attack tables                               */
/* -------------------------------------------------------------------------- */
extern U64 KNIGHT_MOVES[NUM_SQUARES];
extern U64 KING_MOVES[NUM_SQUARES];

#define WHITE 0
#define BLACK 1

// the (max) two possible pawns that could be attacking a square
extern U64 PAWN_ATTACKS[2][NUM_SQUARES];

/* -------------------------------------------------------------------------- */
/*                                   Magics                                   */
/* -------------------------------------------------------------------------- */
extern U64 ROOK_MASKS[NUM_SQUARES];
extern U64 BISHOP_MASKS[NUM_SQUARES];

extern U64 ROOK_MAGICS[NUM_SQUARES];
extern U64 BISHOP_MAGICS[NUM_SQUARES];

extern U8  ROOK_MAGIC_SHIFTS[NUM_SQUARES];
extern U8  BISHOP_MAGIC_SHIFTS[NUM_SQUARES];

extern U64 ROOK_MOVES[NUM_SQUARES][4096];
extern U64 BISHOP_MOVES[NUM_SQUARES][4096];

extern U8 LSB_64_TABLE[NUM_SQUARES];

/* -------------------------------------------------------------------------- */
/*                                 Ranks/Files                                */
/* -------------------------------------------------------------------------- */
enum Ranks {
    Rank1 = 0b00000001,
    Rank2 = 0b00000010,
    Rank3 = 0b00000100,
    Rank4 = 0b00001000,
    Rank5 = 0b00010000,
    Rank6 = 0b00100000,
    Rank7 = 0b01000000,
    Rank8 = 0b10000000
};
enum Files {
    FileA = 0b00000001,
    FileB = 0b00000010,
    FileC = 0b00000100,
    FileD = 0b00001000,
    FileE = 0b00010000,
    FileF = 0b00100000,
    FileG = 0b01000000,
    FileH = 0b10000000
};

/* -------------------------------------------------------------------------- */
/*                                Colors/Pieces                               */
/* -------------------------------------------------------------------------- */
#define PIECE_COLOR     0b1000

enum Pieces {
    Pawn, Knight, Bishop, Rook, Queen, King, All, None
};
enum Colors {
    White = 0b0000,
    Black = 0b1000
};

Colors operator!(Colors color);

/* -------------------------------------------------------------------------- */
/*                            Initialize all tables                           */
/* -------------------------------------------------------------------------- */
void init();

/* -------------------------------------------------------------------------- */
/*                                   Strings                                  */
/* -------------------------------------------------------------------------- */
std::string bb_to_string(U64 bitboard);

/* -------------------------------------------------------------------------- */
/*                            Rank/File to Position                           */
/* -------------------------------------------------------------------------- */
inline U8 calc_pos(int rank, int file) {
    return (7-rank) * 8 + file;
}
inline void calc_rf(U8 pos, int &rank, int &file) {
    file = pos % 8;
    rank = 7-(pos / 8);
}

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
inline bool is_set(const U64 &board, U8 pos) {
    return board & SET_BIT_TABLE[pos];
}
inline void set_pos(U64 &board, U8 pos) {
    board |= SET_BIT_TABLE[pos];
}
inline void clr_pos(U64 &board, U8 pos) {
    board &= CLEAR_BIT_TABLE[pos];
}
inline U8 count_occupied(U64 board) {
    U8 count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
inline U8 pop_lsb(U64 &board) {
    // assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}
inline U8 lsb_idx(U64 &board) {
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}

}

#endif // CHESS_GAME_DEFS_HPP
