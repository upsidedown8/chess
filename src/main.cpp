#include "game/board.hpp"

#include <iostream>

using namespace chess_cpp;

U64 calc_rook_moves(int square, U64 occupancy) {
    occupancy &= ROOK_MASKS[square];
    int idx = (ROOK_MAGICS[square]*occupancy)>>ROOK_MAGIC_SHIFTS[square]; 
    return ROOK_MOVES[square][idx];
}
U64 calc_bishop_moves(int square, U64 occupancy) {
    occupancy &= BISHOP_MASKS[square];
    int idx = (BISHOP_MAGICS[square]*occupancy)>>BISHOP_MAGIC_SHIFTS[square]; 
    return BISHOP_MOVES[square][idx];
}

int main(int argc, char const *argv[]) {
    init();

    Board board;

    U64 occupancy = 0;
    for (U64 bb : board.bitboards) occupancy |= bb;

    std::cout << bb_to_string(occupancy) << std::endl;
    std::cout << bb_to_string(calc_bishop_moves(e4, occupancy)) << std::endl;
    
    return 0;
}
