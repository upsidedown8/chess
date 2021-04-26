#include "game/board.hpp"
#include "game/defs.hpp"
#include "game/movegen.hpp"
#include "game/move.hpp"

using namespace chess_cpp;

U64 Perft(Board &board, int depth, bool displayNodes = false);
U64 Perft(const std::string &fen, int depth, bool displayNodes = false);

#define MAXDEPTH 8

Move MOVES[MAXDEPTH+1][256];

U64 Perft(Board &board, int depth, bool displayNodes) {
    int numMoves = gen_moves(board, MOVES[depth]);

    if (depth <= 1)
        return numMoves;

    U64 nodes = 0;
    UndoInfo info;
    for (int i = 0; i < numMoves; i++) {
        board.make_move(MOVES[depth][i], info);
        U64 childNodes = Perft(board, depth - 1);
        // if (displayNodes)
        //     std::cout << move_to_string(MOVES[depth][i]) << ": " << childNodes << std::endl;
        nodes += childNodes;
        board.undo_move(MOVES[depth][i], info);
    }
    return nodes;
}
U64 Perft(const std::string &fen, int depth, bool displayNodes) {
    Board board(fen);
    return Perft(board, depth, displayNodes);
}


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

    Perft(board.to_fen(), 7, false);

    return 0;
}
