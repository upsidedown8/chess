#include "game/board.hpp"
#include "game/utils.hpp"

#include <iostream>

using namespace chess_cpp;

U64 gen_rook_moves(int start) {
    int rank = start / 8;
    int file = start % 8;
    U64 result = RANKS[rank] | FILES[file];
    chess_cpp::clr_pos(result, start);
    return result;
}
U64 gen_bishop_moves(int start) {
    int rank = start / 8;
    int file = start % 8;
    
}

int main(int argc, char const *argv[]) {
    Board board;

    // U64 pawnsInQuestion = board.m_bitboards[WP] & RANKS[6];
    // U64 singleMoves = (pawnsInQuestion >> 8) & RANKS[5];

    // std::cout << to_string(pawnsInQuestion) << std::endl;
    // std::cout << to_string(singleMoves) << std::endl;

    return 0;
}
