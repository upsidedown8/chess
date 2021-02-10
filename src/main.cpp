#include "game/board.hpp"
#include "game/movegen.hpp"

#include <iostream>

using namespace chess_cpp;

int main(int argc, char const *argv[]) {
    init();

    // std::string fen;
    // std::cout << "Enter FEN: ";
    // getline(std::cin, fen);
    // Board board(fen);

    // auto moves = gen_moves(board);

    // std::cout << "number of moves: " << moves.size() << std::endl;
    // for (int i=0; i < moves.size(); i++) {
    //     std::cout << moves[i].to_string() << ' ';
    // }
    // std::cout << std::endl;

    Board board;

    return 0;
}
