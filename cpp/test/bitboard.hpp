#include <cstdint>
#include <string>
#include <iostream>
#include "game/rle.hpp"

typedef uint64_t U64;

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

class BitBoard {
public:
    U64 W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEENS, W_KINGS;
    U64 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEENS, B_KINGS;

    BitBoard();

    void reset();
    void zeroBoards();
    void fromString(const std::string &board);

    std::string toString();

    U64 getMask(int pos);
    U64 getMask(int rank, int file);

    bool isOccupied(const U64 &board, int pos);
    bool isOccupied(const U64 &board, int rank, int file);

    void setPos(U64 &board, int pos, int val);

    int countOccupied(U64 board);
};