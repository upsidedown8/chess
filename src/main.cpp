#include "game/board.hpp"
#include "game/movegen.hpp"

#include <iostream>

using namespace chess_cpp;

int main(int argc, char const *argv[]) {
    init();

    std::string fen;
    std::cout << "Enter FEN: ";
    getline(std::cin, fen);
    Board board(fen);

    while (true) {
        std::cout << "Board: " << std::endl;
        std::cout << board.to_string() << std::endl;

        std::cout << "Moves: " << std::endl;
        MoveList moveList = gen_moves(board);
        for (int i = 0; i < moveList.size(); i++) {
            std::cout << moveList.moves[i].to_string() << std::endl;
        }

        while (true) {
            std::cout << "Enter move: ";
            std::string move;
            getline(std::cin, move);

            int pos = -1;
            for (int i = 0; i < moveList.size(); i++) {
                if (moveList.moves[i].to_string() == move) {
                    pos = i;
                    break;
                }
            }
            if (pos != -1) {
                std::cout << "Move: " << moveList.moves[pos].to_string() << std::endl;
                board.make_move(moveList.moves[pos]);
                break; 
            }
            std::cout << "Illegal move" << std::endl;
        }
    }
    
    return 0;
}