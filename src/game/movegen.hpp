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
int gen_moves(Board &board, Move *arr);

}

#endif // CHESS_GAME_MOVEGEN_HPP
