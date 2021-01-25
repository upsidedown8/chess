#include "defs.hpp"

#include <sstream>
#include <assert.h>

using namespace chess_cpp;

const U8 LSB_64_TABLE[64] {
    63, 30,  3, 32, 25, 41, 22, 33,
    15, 50, 42, 13, 11, 53, 19, 34,
    61, 29,  2, 51, 21, 43, 45, 10,
    18, 47,  1, 54,  9, 57,  0, 35,
    62, 31, 40,  4, 49,  5, 52, 26,
    60,  6, 23, 44, 46, 27, 56, 16,
     7, 39, 48, 24, 59, 14, 12, 55,
    38, 28, 58, 20, 37, 17, 36,  8
};

// tables
U64 set_bit_table[NUM_SQUARES];
U64 clr_bit_table[NUM_SQUARES];

void chess_cpp::init() {
    // set bit table & clear bit table
    for (size_t i = 0; i < NUM_SQUARES; i++) {
        set_bit_table[i] = 1ULL << i;
        clr_bit_table[i] = ~set_bit_table[i];
    }
}

std::string chess_cpp::bb_to_string(U64 bitboard) {
    U64 mask = 1;

    std::stringstream ss;

    for (int i = 0; i < 8; i++) {
        ss << (8-i) << ' ';
        for (int j = 0; j < 8; j++) {
            ss << (bitboard & mask ? 'x' : '.') << ' ';
            mask <<= 1;
        }
        ss << std::endl;
    }
    ss << "  a b c d e f g h";

    return ss.str();
}

U8 chess_cpp::calc_pos(int rank, int file) {
    return rank * 8 + file;
}
void chess_cpp::calc_rf(U8 pos, int &rank, int &file) {
    file = pos % 8;
    rank = pos / 8;
}

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
bool chess_cpp::is_set(const U64 &board, U8 pos) {
    return board & set_bit_table[pos];
}
void chess_cpp::set_pos(U64 &board, U8 pos) {
    board |= set_bit_table[pos];
}
void chess_cpp::clr_pos(U64 &board, U8 pos) {
    board &= clr_bit_table[pos];
}
U8 chess_cpp::count_occupied(U64 board) {
    U8 count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
U8 chess_cpp::pop_lsb(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}
