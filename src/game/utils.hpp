#if !defined(CHESS_GAME_UTILS_HPP)
#define CHESS_GAME_UTILS_HPP

#include "defs.hpp"

#include <string>

namespace chess_cpp {

void init_tables();

/* -------------------------------------------------------------------------- */
/*                               Zobrist hashing                              */
/* -------------------------------------------------------------------------- */
U64 hash(Pieces pieces[NUM_SQUARES]);

/* -------------------------------------------------------------------------- */
/*                                 Conversions                                */
/* -------------------------------------------------------------------------- */
char get_notation(Pieces piece);
Pieces from_notation(char c);
Pieces from_color(int piece, Colors color);

std::string to_string(U64 bitboard);
std::string to_string(Pieces piece);

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
bool is_set(const U64 &board, uchar pos);

void set_pos(U64 &board, uchar pos);
void clr_pos(U64 &board, uchar pos);

uchar count_occupied(U64 board);
uchar pop_lsb(U64 &board);

/* -------------------------------------------------------------------------- */
/*                                     RLE                                    */
/* -------------------------------------------------------------------------- */
std::string rle_encode(const std::string& text);
std::string rle_decode(const std::string& text);

}

#endif // CHESS_GAME_UTILS_HPP
