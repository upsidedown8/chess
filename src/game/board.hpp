#ifndef CHESS_GAME_BOARD_HPP
#define CHESS_GAME_BOARD_HPP

#include "defs.hpp"
#include <string>

#define WHITE_CASTLE_QS 0b00010
#define WHITE_CASTLE_KS 0b00100
#define BLACK_CASTLE_QS 0b01000
#define BLACK_CASTLE_KS 0b10000

namespace chess_cpp {

class Board {
private:
    void zero_boards();
    bool from_string(const std::string &str);

public:
    bool white_to_move;
    U8 castling;
    U8 half_move_count;
    U8 full_move_count;
    U8 en_passant;

    U64 bitboards[16];

    Board();
    Board(const std::string &str);

    void reset();

    std::string to_fen();
    std::string to_string();
};

}

#endif // CHESS_GAME_BOARD_HPP
