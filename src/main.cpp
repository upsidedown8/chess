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
    std::cout << board.to_string() << std::endl;
    
    U8 flags = 0;
    Move move(e2, e4, flags);

    board.make_move(move);

    std::cout << board.to_string() << std::endl;

    return 0;
}
