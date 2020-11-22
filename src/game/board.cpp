#include "game/board.hpp"

#include "utils.hpp"

#include <assert.h>
#include <sstream>

using namespace chess_cpp;

void Board::zero_boards() {
    for (int piece = WP; piece <= BK; piece++)
        m_bitboards[piece] = 0ULL;
}
void Board::update_bitboards() {
    for (int piece = WP; piece <= WK; piece++)
        m_combined[piece] =
            m_bitboards[from_color(piece, WHITE)] |
            m_bitboards[from_color(piece, BLACK)];

    for (size_t i = 0; i < NUM_SQUARES; i++)
        m_pieces[i] = EMPTY;

    uchar pos;
    U64 bb;
    for (int piece = WP; piece <= BK; piece++) {
        bb = m_bitboards[piece];

        while (bb) {
            pos = pop_lsb(bb);
            m_pieces[pos] = (Pieces)piece;
        }
    }
}
void Board::from_string(const std::string &str) {
    zero_boards();
    std::string boardDecoded = rle_decode(str);

    assert(boardDecoded.length() == NUM_SQUARES);

    Pieces piece;
    U64 shift = 1ULL;
    for (int i = 0; i < NUM_SQUARES; i++, shift <<= 1) {
        piece = from_notation(boardDecoded[i]);
        if (piece >= WP && piece <= BK) {
            m_bitboards[piece] |= shift;
        }
    }
    update_bitboards();
}

Board::Board() {
    init_tables();
    reset();
}
Board::Board(const std::string &str) {
    init_tables();
    from_string(str);
}

void Board::reset() {
    from_string("rnbqkbnr8p32.8PRNBQKBNR");
}

std::string Board::to_string(bool prettyPrint) {
    update_bitboards();

    if (prettyPrint) {
        std::stringstream ss;
        ss << "  ";
        for (char i = 'A'; i <= 'H'; i++)
            ss << i << ' ';
        ss << std::endl;

        for (int i = 0; i < 8; i++) {
            ss << std::to_string(8-i) << ' ';
            for (int j = 0; j < 8; j++)
                ss << get_notation(m_pieces[i * 8 + j]) << ' ';
            ss << std::endl;
        }

        return ss.str();
    } else {
        std::string result(NUM_SQUARES, '\0');
        for (int i = 0; i < NUM_SQUARES; i++)
            result[i] = get_notation(m_pieces[i]);

        return rle_encode(result);
    }
}
