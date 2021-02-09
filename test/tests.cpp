#include <gtest/gtest.h>

#include "game/board.hpp"
#include "game/defs.hpp"
#include "game/movegen.hpp"
#include "game/move.hpp"

using namespace chess_cpp;

U64 Perft(Board &board, int depth, bool displayNodes = false);
U64 Perft(const std::string &fen, int depth, bool displayNodes = false);

U64 Perft(Board &board, int depth, bool displayNodes) {
    auto move_list = gen_moves(board);

    if (depth <= 1)
        return move_list.size();

    U64 nodes = 0;
    for (int i = 0; i < move_list.size(); i++) {
        UndoInfo info = board.make_move(move_list[i]);
        U64 childNodes = Perft(board, depth - 1);
        if (displayNodes)
            std::cout << move_list[i].to_string() << ": " << childNodes << std::endl;
        nodes += childNodes;
        board.undo_move(move_list[i], info);
    }
    return nodes;
}
U64 Perft(const std::string &fen, int depth, bool displayNodes) {
    Board board(fen);
    return Perft(board, depth, displayNodes);
}

void fen_test(const std::string &fen) {
    Board board(fen);
    ASSERT_EQ(board.to_fen(), fen) << board.to_string() << std::endl;
}
TEST(BoardTests, FEN) {
    fen_test("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b QK - 3 2");
    fen_test("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3");
    fen_test("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w QqKk - 2 2");
    fen_test("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b QqKk - 3 2");
    fen_test("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w QK - 3 9");
    fen_test("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4");
    fen_test("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b QK - 3 2");
    fen_test("4k3/8/8/5R2/8/8/8/4K3 b - - 0 1");
    fen_test("8/4k3/8/8/4R3/8/8/4K3 b - - 0 1");
    fen_test("4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1");
    fen_test("4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1");
    fen_test("8/8/8/2k5/3Pp3/8/8/4K3 b - d3 0 1");
    fen_test("8/8/8/1k6/3Pp3/8/8/4KQ2 b - d3 0 1");
    fen_test("4k3/8/4r3/8/8/4Q3/8/2K5 b - - 0 1");
    fen_test("8/8/8/8/k2Pp2Q/8/8/2K5 b - d3 0 1");
}

void undo_test(const std::string &fen, int &num) {    
    Board board(fen);

    for (Move &move : gen_moves(board)) {
        Board testBoard(fen);

        UndoInfo info = testBoard.make_move(move);
        testBoard.undo_move(move, info);
        ASSERT_TRUE(testBoard == board) << "test no: " << num << " move: " << move.to_string();
    }
    ++num;
}
TEST(BoardTests, UndoMove) {
    int num = 0;
    undo_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1R1K b kq - 1 1", num);
    undo_test("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b QK - 3 2", num);
    undo_test("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 5 3", num);
    undo_test("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w QqKk - 2 2", num);
    undo_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", num);
    undo_test("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b QqKk - 3 2", num);
    undo_test("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w QK - 3 9", num);
    undo_test("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4", num);
    undo_test("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b QK - 3 2", num);
    undo_test("4k3/8/8/5R2/8/8/8/4K3 b - - 0 1", num);
    undo_test("8/4k3/8/8/4R3/8/8/4K3 b - - 0 1", num);
    undo_test("4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1", num);
    undo_test("4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1", num);
    undo_test("8/8/8/2k5/3Pp3/8/8/4K3 b - d3 0 1", num);
    undo_test("8/8/8/1k6/3Pp3/8/8/4KQ2 b - d3 0 1", num);
    undo_test("4k3/8/4r3/8/8/4Q3/8/2K5 b - - 0 1", num);
    undo_test("8/8/8/8/k2Pp2Q/8/8/2K5 b - d3 0 1", num);
    undo_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", num);
    undo_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", num);
    undo_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", num);
    undo_test("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", num);
    undo_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", num);
    undo_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", num);
    undo_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", num);
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
    ASSERT_EQ(Perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1), 44);
    ASSERT_EQ(Perft("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 1), 46);
    ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1), 6);
    ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 1), 6);
    ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1), 14);
    ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1), 48);
    ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1), 20);
}
TEST(Perft, Depth2) {
    ASSERT_EQ(Perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2), 1486);
    ASSERT_EQ(Perft("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 2), 2079);
    ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2), 264);
    ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 2), 264);
    ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -", 2), 191);
    ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -", 2), 2039);
    ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2), 400);
}
TEST(Perft, Depth3) {
    ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3), 2812);
    ASSERT_EQ(Perft("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3), 89890);
    ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3), 9467);
    ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 3), 9467);
    ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3), 8902);
    ASSERT_EQ(Perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3), 62379);
    ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3), 97862);
}
TEST(Perft, Depth4) {
    ASSERT_EQ(Perft("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4), 1274206);
    ASSERT_EQ(Perft("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4), 1720476);
    ASSERT_EQ(Perft("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4), 23527);
    ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4), 422333);
    ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 4), 422333);
    ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4), 43238);
    ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4), 4085603);
    ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4), 197281);
}
TEST(Perft, Depth5) {
    ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5), 15833292);
    // ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5), 4865609);
    // ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 5, true), 15833292);
    // ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5), 674624);
    // ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5), 193690690);
    // ASSERT_EQ(Perft("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5), 1004658);
}
// TEST(Perft, Depth6) {h
//     ASSERT_EQ(Perft("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6), 1134888);
//     ASSERT_EQ(Perft("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 6), 1015133);
//     ASSERT_EQ(Perft("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6), 1440467);
//     ASSERT_EQ(Perft("5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6), 661072);
//     ASSERT_EQ(Perft("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6), 803711);
//     ASSERT_EQ(Perft("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6), 3821001);
//     ASSERT_EQ(Perft("4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6), 217342);
//     ASSERT_EQ(Perft("8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6), 92683);
//     ASSERT_EQ(Perft("K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6), 2217);
//     ASSERT_EQ(Perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6), 706045033);
//     ASSERT_EQ(Perft("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 6), 706045033);
//     ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6), 11030083);
//     ASSERT_EQ(Perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 6), 8031647685);
//     ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6), 119060324);
// }
// TEST(Perft, Depth7) {
//     ASSERT_EQ(Perft("8/k1P5/8/1K6/8/8/8/8 w - - 0 1", 7), 567584);
//     ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 7), 178633661);
//     ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 7), 3195901860);
// }
// TEST(Perft, Depth8) {
//     ASSERT_EQ(Perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 8), 3009794393);
//     ASSERT_EQ(Perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 8), 84998978956);
// }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    chess_cpp::init();

    return RUN_ALL_TESTS();
}
