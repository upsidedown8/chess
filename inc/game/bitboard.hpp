#pragma once
#include <assert.h>
#include <cstdint>
#include <string>
#include <iostream>
#include "game/rle.hpp"

typedef uint64_t U64;
typedef unsigned char uchar;

const uchar LSB_64_TABLE[64] = {
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};
const U64 SET_BIT_TABLE[64] = {
    1ULL<< 0,1ULL<< 1,1ULL<< 2,1ULL<< 3,1ULL<< 4,1ULL<< 5,1ULL<< 6,1ULL<< 7,
    1ULL<< 8,1ULL<< 9,1ULL<<10,1ULL<<11,1ULL<<12,1ULL<<13,1ULL<<14,1ULL<<15,
    1ULL<<16,1ULL<<17,1ULL<<18,1ULL<<19,1ULL<<20,1ULL<<21,1ULL<<22,1ULL<<23,
    1ULL<<24,1ULL<<25,1ULL<<26,1ULL<<27,1ULL<<28,1ULL<<29,1ULL<<30,1ULL<<31,
    1ULL<<32,1ULL<<33,1ULL<<34,1ULL<<35,1ULL<<36,1ULL<<37,1ULL<<38,1ULL<<39,
    1ULL<<40,1ULL<<41,1ULL<<42,1ULL<<43,1ULL<<44,1ULL<<45,1ULL<<46,1ULL<<47,
    1ULL<<48,1ULL<<49,1ULL<<50,1ULL<<51,1ULL<<52,1ULL<<53,1ULL<<54,1ULL<<55,
    1ULL<<56,1ULL<<57,1ULL<<58,1ULL<<59,1ULL<<60,1ULL<<61,1ULL<<62,1ULL<<63
};
const U64 CLR_BIT_TABLE[64] = {
    ~(1ULL<< 0),~(1ULL<< 1),~(1ULL<< 2),~(1ULL<< 3),~(1ULL<< 4),~(1ULL<< 5),~(1ULL<< 6),~(1ULL<< 7),
    ~(1ULL<< 8),~(1ULL<< 9),~(1ULL<<10),~(1ULL<<11),~(1ULL<<12),~(1ULL<<13),~(1ULL<<14),~(1ULL<<15),
    ~(1ULL<<16),~(1ULL<<17),~(1ULL<<18),~(1ULL<<19),~(1ULL<<20),~(1ULL<<21),~(1ULL<<22),~(1ULL<<23),
    ~(1ULL<<24),~(1ULL<<25),~(1ULL<<26),~(1ULL<<27),~(1ULL<<28),~(1ULL<<29),~(1ULL<<30),~(1ULL<<31),
    ~(1ULL<<32),~(1ULL<<33),~(1ULL<<34),~(1ULL<<35),~(1ULL<<36),~(1ULL<<37),~(1ULL<<38),~(1ULL<<39),
    ~(1ULL<<40),~(1ULL<<41),~(1ULL<<42),~(1ULL<<43),~(1ULL<<44),~(1ULL<<45),~(1ULL<<46),~(1ULL<<47),
    ~(1ULL<<48),~(1ULL<<49),~(1ULL<<50),~(1ULL<<51),~(1ULL<<52),~(1ULL<<53),~(1ULL<<54),~(1ULL<<55),
    ~(1ULL<<56),~(1ULL<<57),~(1ULL<<58),~(1ULL<<59),~(1ULL<<60),~(1ULL<<61),~(1ULL<<62),~(1ULL<<63)
};

const int NUMBER_OF_SQUARES = 64;

const char START_POSITION[] = "rnbqkbnr8p32#8PRNBQKBNR";

const char B_PAWN_NOTATION   = 'p';
const char B_KNIGHT_NOTATION = 'n';
const char B_BISHOP_NOTATION = 'b';
const char B_ROOK_NOTATION   = 'r';
const char B_QUEEN_NOTATION  = 'q';
const char B_KING_NOTATION   = 'k';

const char W_PAWN_NOTATION   = 'P';
const char W_KNIGHT_NOTATION = 'N';
const char W_BISHOP_NOTATION = 'B';
const char W_ROOK_NOTATION   = 'R';
const char W_QUEEN_NOTATION  = 'Q';
const char W_KING_NOTATION   = 'K';

const char EMPTY_NOTATION    = '#';

const U64 FILE_1 = 0x0101010101010101L;
const U64 FILE_2 = 0x0202020202020202L;
const U64 FILE_3 = 0x0404040404040404L;
const U64 FILE_4 = 0x0808080808080808L;
const U64 FILE_5 = 0x1010101010101010L;
const U64 FILE_6 = 0x2020202020202020L;
const U64 FILE_7 = 0x4040404040404040L;
const U64 FILE_8 = 0x8080808080808080L;

const U64 RANK_A = 0x00000000000000ffL;
const U64 RANK_B = 0x000000000000ff00L;
const U64 RANK_C = 0x0000000000ff0000L;
const U64 RANK_D = 0x00000000ff000000L;
const U64 RANK_E = 0x000000ff00000000L;
const U64 RANK_F = 0x0000ff0000000000L;
const U64 RANK_G = 0x00ff000000000000L;
const U64 RANK_H = 0xff00000000000000L;

typedef struct {
    U64 W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEENS, W_KINGS;
    U64 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEENS, B_KINGS;
} BitBoard;

namespace BitBoardUtils {
    void zeroBoards(BitBoard &board);
    void bbFromString(BitBoard &result, const std::string &board);

    std::string bbToString(BitBoard &board);

    bool isSet(const U64 &board, uchar pos);

    void setPos(U64 &board, uchar pos);
    void clrPos(U64 &board, uchar pos);

    uchar countOccupied(U64 board);
    uchar popLSB(U64 &board);
}