#include "utils.hpp"

#include <sstream>
#include <assert.h>

using namespace chess_cpp;

const uchar LSB_64_TABLE[64] = {
    63, 30,  3, 32, 25, 41, 22, 33,
    15, 50, 42, 13, 11, 53, 19, 34,
    61, 29,  2, 51, 21, 43, 45, 10,
    18, 47,  1, 54,  9, 57,  0, 35,
    62, 31, 40,  4, 49,  5, 52, 26,
    60,  6, 23, 44, 46, 27, 56, 16,
     7, 39, 48, 24, 59, 14, 12, 55,
    38, 28, 58, 20, 37, 17, 36,  8
};
const std::string NOTATION = "PNBRQKpnbrqk.";

// tables
U64 rand_table[NUM_SQUARES][12];
U64 set_bit_table[NUM_SQUARES];
U64 clr_bit_table[NUM_SQUARES];

bool initialized = false;

void chess_cpp::init_tables() {
    if (!initialized) {
        // set bit table & clear bit table
        for (size_t i = 0; i < NUM_SQUARES; i++) {
            set_bit_table[i] = 1ULL << i;
            clr_bit_table[i] = ~set_bit_table[i];
        }

        // Zobrist hashing table
        for (int i = 0; i < NUM_SQUARES; i++) {
            for (int j = 0; j < 12; j++) {
                // ensure the random number fills all 64 bits
                rand_table[i][j] =
                    ((U64)rand() & 0x7fff)       +
                    ((U64)rand() & 0x7fff) << 15 + 
                    ((U64)rand() & 0x7fff) << 30 + 
                    ((U64)rand() & 0x7fff) << 45 +
                    ((U64)rand() & 0xf)    << 60;
            }
        }
        initialized = true;
    }
}

/* -------------------------------------------------------------------------- */
/*                               Zobrist hashing                              */
/* -------------------------------------------------------------------------- */
U64 chess_cpp::hash(Pieces pieces[NUM_SQUARES]) {
    init_tables();
    U64 h = 0ULL;
    for (uchar i = 0; i < NUM_SQUARES; i++)
        if (pieces[i] != EMPTY)
            h ^= rand_table[i][pieces[i]];
    return h;
}

/* -------------------------------------------------------------------------- */
/*                                 Conversions                                */
/* -------------------------------------------------------------------------- */
char chess_cpp::get_notation(Pieces piece) {
    assert(piece >= 0 && piece < NOTATION.length());
    return NOTATION[piece];
}
Pieces chess_cpp::from_notation(char c) {
    size_t pos = NOTATION.find(c);
    assert(pos != std::string::npos);
    return (Pieces)pos;
}
Pieces chess_cpp::from_color(int piece, Colors color) {
    return (Pieces)(piece + (color == WHITE ? WP : BP));
}

std::string chess_cpp::to_string(U64 bitboard) {
    U64 mask = 1;

    std::stringstream ss;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ss << (bitboard & mask ? 'x' : '.') << ' ';
            mask <<= 1;
        }
        ss << std::endl;
    }

    return ss.str();
}
std::string chess_cpp::to_string(Pieces piece) {
    switch (piece) {
        case WP: return "white pawn";
        case WN: return "white knight";
        case WB: return "white bishop";
        case WR: return "white rook";
        case WQ: return "white queen";
        case WK: return "white king";

        case BP: return "black pawn";
        case BN: return "black knight";
        case BB: return "black bishop";
        case BR: return "black rook";
        case BQ: return "black queen";
        case BK: return "black king";

        default: return "empty";
    }
}

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
bool chess_cpp::is_set(const U64 &board, uchar pos) {
    init_tables();
    return board & set_bit_table[pos];
}

void chess_cpp::set_pos(U64 &board, uchar pos) {
    init_tables();
    board |= set_bit_table[pos];
}
void chess_cpp::clr_pos(U64 &board, uchar pos) {
    init_tables();
    board &= clr_bit_table[pos];
}

uchar chess_cpp::count_occupied(U64 board) {
    uchar count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
uchar chess_cpp::pop_lsb(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}

/* -------------------------------------------------------------------------- */
/*                                     RLE                                    */
/* -------------------------------------------------------------------------- */
std::string chess_cpp::rle_encode(const std::string& text) {
    std::stringstream ss;
    for (size_t idx = 0; idx < text.length(); idx++){
        int rl = 1;
        while (idx+1 < text.length() && text[idx] == text[idx+1]){
            rl++;
            idx++;
        }
        if (rl > 1)
            ss << rl;
        ss << text[idx];
    }
    return ss.str();
}
std::string chess_cpp::rle_decode(const std::string& text) {
    std::stringstream ss;
    size_t idx = 0;
    while (idx < text.length()) {
        int digits = 0;
        while(isdigit(text[idx + digits]))
            digits++;
        int runLength = digits > 0 ? stoi(text.substr(idx, idx + digits)) : 1;
        for (int x = 0; x < runLength; x++)
            ss << text[idx + digits];
        idx += digits + 1;
    }
    return ss.str();
}
