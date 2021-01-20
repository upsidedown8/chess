#if !defined(CHESS_GAME_UTILS_HPP)
#define CHESS_GAME_UTILS_HPP

#include "defs.hpp"

#include <string>

namespace chess_cpp {

void init_tables();

std::string bb_to_string(U64 bitboard);

/* -------------------------------------------------------------------------- */
/*                             Bitwise operations                             */
/* -------------------------------------------------------------------------- */
bool is_set(const U64 &board, U8 pos);

void set_pos(U64 &board, U8 pos);
void clr_pos(U64 &board, U8 pos);

U8 count_occupied(U64 board);
U8 pop_lsb(U64 &board);

}

#endif // CHESS_GAME_UTILS_HPP
