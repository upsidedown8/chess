#ifndef CHESS_GAME_MOVEGEN_HPP
#define CHESS_GAME_MOVEGEN_HPP

#include "move.hpp"
#include "board.hpp"
#include "defs.hpp"

#include <vector>

namespace chess_cpp {

struct MoveList {
    Move moves[256];
    Move *ptr;
    U64 attacks;

    MoveList();

    size_t size();
};

/* -------------------------------------------------------------------------- */
/*                               Move generation                              */
/* -------------------------------------------------------------------------- */
MoveList gen_moves(Board &board, U64 enemyAttacks);

}

#endif // CHESS_GAME_MOVEGEN_HPP
