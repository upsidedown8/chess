#include "game/board.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
    chess_cpp::init();
    // chess_cpp::Board board;

    // std::cout << board.to_string() << std::endl;

    std::cout << chess_cpp::bb_to_string(chess_cpp::BISHOP_MASKS[chess_cpp::f6]) << std::endl;

    return 0;
}
