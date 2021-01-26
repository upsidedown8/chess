#include "defs.hpp"

#include <sstream>
#include <assert.h>

chess_cpp::U64 chess_cpp::RANKS[256];
chess_cpp::U64 chess_cpp::FILES[256];
chess_cpp::U64 chess_cpp::NOT_RANKS[256];
chess_cpp::U64 chess_cpp::NOT_FILES[256];

chess_cpp::U64 chess_cpp::ROOK_MASKS[NUM_SQUARES];
chess_cpp::U64 chess_cpp::BISHOP_MASKS[NUM_SQUARES];

const chess_cpp::U8 LSB_64_TABLE[64] {
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
chess_cpp::U64 SET_BIT_TABLE[NUM_SQUARES];
chess_cpp::U64 CLEAR_BIT_TABLE[NUM_SQUARES];

/* -------------------------------------------------------------------------- */
/*                            Initialize all tables                           */
/* -------------------------------------------------------------------------- */
chess_cpp::U64 gen_rook_moves(int start, chess_cpp::U64 occupancy) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1; r>=0; r--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,file));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,file)))
            break;
    }
    for (int r=rank+1; r<=7; r++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,file));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,file)))
            break;
    }
    for (int f=file-1; f>=0; f--) {
        result |= (1ULL<<chess_cpp::calc_pos(rank,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(rank,f)))
            break;
    }
    for (int f=file+1; f<=7; f++) {
        result |= (1ULL<<chess_cpp::calc_pos(rank,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(rank,f)))
            break;
    }

    return result;
}
chess_cpp::U64 gen_bishop_moves(int start, chess_cpp::U64 occupancy) {
    int rank, file;
    chess_cpp::calc_rf(start, rank, file);
    chess_cpp::U64 result = 0;

    for (int r=rank-1, f=file-1; r>=0&&f>=0; r--, f--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank+1, f=file-1; r<=7&&f>=0; r++, f--) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank-1, f=file+1; r>=0&&f<=7; r--, f++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }
    for (int r=rank+1, f=file+1; r<=7&&f<=7; r++, f++) {
        result |= (1ULL<<chess_cpp::calc_pos(r,f));
        if (occupancy & (1ULL<<chess_cpp::calc_pos(r,f)))
            break;
    }

    return result;
}

void chess_cpp::init() {
    // set bit table & clear bit table
    for (size_t i = 0; i < NUM_SQUARES; i++) {
        SET_BIT_TABLE[i] = 1ULL << i;
        CLEAR_BIT_TABLE[i] = ~SET_BIT_TABLE[i];
    }

    // ranks
    for (int i = 0; i < 8; i++)
        RANKS[1<<i] = 0xffULL<<(8*(7-i));
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 8; j++)
            if (i & (1<<j))
                RANKS[i] |= RANKS[1<<j];
        NOT_RANKS[i] = ~RANKS[i];
    }

    // files
    for (int i = 0; i < 8; i++)
        FILES[1<<i] = 0x0101010101010101UL<<i;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 8; j++)
            if (i & (1<<j))
                FILES[i] |= FILES[1<<j];
        NOT_FILES[i] = ~FILES[i];
    }

    // rook masks
    for (int i = 0; i < NUM_SQUARES; i++)
        ROOK_MASKS[i] = gen_rook_moves(i, 0);

    // bishop masks
    for (int i = 0; i < NUM_SQUARES; i++)
        BISHOP_MASKS[i] = gen_rook_moves(i, 0);
}

/* -------------------------------------------------------------------------- */
/*                                   Strings                                  */
/* -------------------------------------------------------------------------- */
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

/* -------------------------------------------------------------------------- */
/*                            Rank/File to Position                           */
/* -------------------------------------------------------------------------- */
chess_cpp::U8 chess_cpp::calc_pos(int rank, int file) {
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
    return board & SET_BIT_TABLE[pos];
}
void chess_cpp::set_pos(U64 &board, U8 pos) {
    board |= SET_BIT_TABLE[pos];
}
void chess_cpp::clr_pos(U64 &board, U8 pos) {
    board &= CLEAR_BIT_TABLE[pos];
}
chess_cpp::U8 chess_cpp::count_occupied(U64 board) {
    U8 count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
chess_cpp::U8 chess_cpp::pop_lsb(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x783A9B23) >> 26];
}
