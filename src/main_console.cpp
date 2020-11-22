#include "game/board.hpp"
#include "game/utils.hpp"

#include <iostream>

void display_bitboards(chess_cpp::Board &board) {
    for (int piece = chess_cpp::WP; piece <= chess_cpp::BK; piece++) {
        std::cout
            << "bitboard: "
            << chess_cpp::to_string((chess_cpp::Pieces)piece)
            << std::endl;
        std::cout
            << chess_cpp::to_string(board.m_bitboards[piece])
            << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    chess_cpp::Board board;

    display_bitboards(board);
    
    std::cout << board.to_string(true);

    return 0;
}
