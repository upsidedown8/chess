#ifndef CHESS_GAME_MOVEGEN_HPP
#define CHESS_GAME_MOVEGEN_HPP

#include "move.hpp"
#include "board.hpp"
#include "defs.hpp"

#include <vector>

namespace chess_cpp {

/* -------------------------------------------------------------------------- */
/*                               Move generation                              */
/* -------------------------------------------------------------------------- */
std::vector<Move> gen_moves(Board &board);

}

#endif // CHESS_GAME_MOVEGEN_HPP
