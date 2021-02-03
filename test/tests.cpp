#include <gtest/gtest.h>

#include "game/board.hpp"
#include "game/defs.hpp"
#include "game/movegen.hpp"
#include "game/move.hpp"

using namespace chess_cpp;

U64 Perft(Board &board, int depth);
U64 Perft(const std::string &fen, int depth);

U64 Perft(Board &board, int depth) {
    auto move_list = gen_moves(board);

    if (depth <= 1)
        return move_list.size();

    U64 nodes = 0;
    for (int i = 0; i < move_list.size(); i++) {
        board.make_move(move_list[i]);
        nodes += Perft(board, depth - 1);
        board.undo_move(move_list[i]);
    }
    return nodes;
}
U64 Perft(const std::string &fen, int depth) {
    Board board(fen);
    return Perft(board, depth);
}

TEST(Perft, Depth1) {
    ASSERT_EQ(Perft("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b QK - 3 2", 1), 8);
    ASSERT_EQ(Perft("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3", 1), 8);
    ASSERT_EQ(Perft("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w QqKk - 2 2", 1), 19);
    ASSERT_EQ(Perft("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b QqKk - 3 2", 1), 5);
    ASSERT_EQ(Perft("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w QK - 3 9", 1), 39);
    ASSERT_EQ(Perft("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4", 1), 9);
    ASSERT_EQ(Perft("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b QK - 3 2", 1), 44);
    ASSERT_EQ(Perft("4k3/8/8/5R2/8/8/8/4K3 b - - 0 1", 1), 3);
    ASSERT_EQ(Perft("8/4k3/8/8/4R3/8/8/4K3 b - - 0 1", 1), 6);
    ASSERT_EQ(Perft("4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1", 1), 4);
    ASSERT_EQ(Perft("4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1", 1), 6);
    ASSERT_EQ(Perft("8/8/8/2k5/3Pp3/8/8/4K3 b - d3 0 1", 1), 9);
    ASSERT_EQ(Perft("8/8/8/1k6/3Pp3/8/8/4KQ2 b - d3 0 1", 1), 6);
    ASSERT_EQ(Perft("4k3/8/4r3/8/8/4Q3/8/2K5 b - - 0 1", 1), 9);
    ASSERT_EQ(Perft("8/8/8/8/k2Pp2Q/8/8/2K5 b - d3 0 1", 1), 6);
}
TEST(Perft, Depth3) {
    ASSERT_EQ(Perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3), 62379);
    ASSERT_EQ(Perft("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3), 89890);
}
TEST(Perft, Depth4) {
    ASSERT_EQ(Perft("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4), 1274206);
    ASSERT_EQ(Perft("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4), 1720476);
    ASSERT_EQ(Perft("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4), 23527);
}
TEST(Perft, Depth5) {
    ASSERT_EQ(Perft("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5), 1004658);
}
TEST(Perft, Depth6) {
    ASSERT_EQ(Perft("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6), 1134888);
    ASSERT_EQ(Perft("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 6), 1015133);
    ASSERT_EQ(Perft("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6), 1440467);
    ASSERT_EQ(Perft("5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6), 661072);
    ASSERT_EQ(Perft("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6), 803711);
    ASSERT_EQ(Perft("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6), 3821001);
    ASSERT_EQ(Perft("4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6), 217342);
    ASSERT_EQ(Perft("8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6), 92683);
    ASSERT_EQ(Perft("K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6), 2217);
}
TEST(Perft, Depth7) {
    ASSERT_EQ(Perft("8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 7), 567584);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    chess_cpp::init();

    return RUN_ALL_TESTS();
}
