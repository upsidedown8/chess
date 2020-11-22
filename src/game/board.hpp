#if !defined(CHESS_GAME_BOARD_HPP)
#define CHESS_GAME_BOARD_HPP

#include "defs.hpp"
#include <string>

namespace chess_cpp {

class Board {
private:
    void zero_boards();
    void update_bitboards();
    void from_string(const std::string &str);

public:
    uchar m_game_state;

    U64 m_bitboards[12];
    U64 m_combined[6];

    Pieces m_pieces[NUM_SQUARES];

    Board();
    Board(const std::string &str);

    void reset();

    std::string to_string(bool prettyPrint);
};

}

#endif // CHESS_GAME_BOARD_HPP
