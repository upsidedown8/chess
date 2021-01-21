#if !defined(CHESS_GAME_MOVEGEN_HPP)
#define CHESS_GAME_MOVEGEN_HPP

#include "defs.hpp"
#include "move.hpp"
#include <vector>

namespace chess_cpp {

std::vector<Move> gen_moves(Board &board);



}

#endif // CHESS_GAME_MOVEGEN_HPP
