#ifndef CHESS_GAME_BOARD_HPP
#define CHESS_GAME_BOARD_HPP

#include "defs.hpp"
#include "move.hpp"
#include <string>

#define BLACK_CASTLE_QS 0b0001
#define BLACK_CASTLE_KS 0b0010
#define WHITE_CASTLE_QS 0b0100
#define WHITE_CASTLE_KS 0b1000

namespace chess_cpp {

class Board {
private:
    void zero_boards();
    bool from_string(const std::string &str);
    
public:
    bool white_to_move;
    U8 castling;
    U8 fifty_move;
    U8 full_move_count;
    U8 en_passant;

    int pieces[NUM_SQUARES];
    U64 bitboards[16];

    Board();
    Board(const std::string &str);

    void reset();
    void update_bitboards();
    UndoInfo make_move(Move &move);
    void undo_move(Move &move, UndoInfo &info);

    Colors active_color();
    Colors enemy_color();

    std::string to_fen();
    std::string to_string();

    bool operator==(Board &other);
};

}

#endif // CHESS_GAME_BOARD_HPP
