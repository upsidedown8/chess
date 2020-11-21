#include "game/board.hpp"

#include <sstream>
#include <assert.h>

const uchar LSB_64_TABLE[64] = {
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};
const std::string NOTATION = "PNBRQKpnbrqk#";

U64 Board::hash() {
    U64 h = 0ULL;
    // for (uchar i = 0; i < NUM_SQUARES; i++)
    //     if (PIECES[i] != EMPTY)
    //         h ^= m_rand_table[i][PIECES[i]];
    return h;
}

char Board::get_notation(Pieces piece, Colors color) {
    size_t pos = piece + (color != WHITE) * 6;
    assert(pos >= 0 && pos < NOTATION.length());
    return NOTATION[pos];
}
void Board::from_notation(char c, Pieces *piece, Colors *color) {
    size_t pos = NOTATION.find(c);
    assert(pos != std::string::npos);
    *piece = pos == NOTATION.length() -1
        ? EMPTY
        : (Pieces)(pos % 6);
    *color = *piece == EMPTY
        ? NONE
        : (pos > 5
            ? BLACK
            : WHITE
        );
}

void Board::init_tables() {
    // set bit table & clear bit table

    for (size_t i = 0; i < NUM_SQUARES; i++) {
        m_set_bit_table[i] = 1ULL << i;
        m_clr_bit_table[i] = ~m_set_bit_table[i];
    }

    // Zobrist hashing table
    for (int i = 0; i < NUM_SQUARES; i++) {
        for (int j = 0; j < 12; j++) {
            // ensure the random number fills all 64 bits
            m_rand_table[i][j] =
                ((U64)rand() & 0x7fff)       +
                ((U64)rand() & 0x7fff) << 15 + 
                ((U64)rand() & 0x7fff) << 30 + 
                ((U64)rand() & 0x7fff) << 45 +
                ((U64)rand() & 0xf)    << 60;
        }
    }
}
void Board::zero_boards() {
    for (int color = WHITE; color <= ALL; color++)
        for (int piece = PAWN; piece <= KING; piece++)
            m_bitboards[color][piece] = 0ULL;
}
void Board::update_bitboards() {
    for (int piece = PAWN; piece <= KING; piece++)
        m_bitboards[ALL][piece] =
            m_bitboards[WHITE][piece] |
            m_bitboards[BLACK][piece];
}
void Board::from_string(const std::string &str) {
    zero_boards();
    std::string boardDecoded = rle_decode(str);

    assert(boardDecoded.length() == NUM_SQUARES);

    Colors color;
    Pieces piece;
    U64 shift = 1ULL;
    for (int i = 0; i < NUM_SQUARES; i++, shift <<= 1) {
        from_notation(boardDecoded[i], &piece, &color);
        if (piece >= PAWN && piece <= KING &&
            color >= WHITE && color <= BLACK) {
                m_bitboards[color][piece] |= shift;
        }
    }
}

bool Board::is_set(const U64 &board, uchar pos) {
    return board & m_set_bit_table[pos];
}

void Board::set_pos(U64 &board, uchar pos) {
    board |= m_set_bit_table[pos];
}
void Board::clr_pos(U64 &board, uchar pos) {
    board &= m_clr_bit_table[pos];
}

uchar Board::count_occupied(U64 board) {
    uchar count = 0;
    while (board) {
        board &= board-1;
        count++;
    }
    return count;
}
uchar Board::pop_lsb(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    U64 folded = (unsigned)((b & 0xffffffff) ^ (board >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x78291ACF) >> 26];
}

std::string Board::rle_encode(const std::string& text) {
    std::stringstream ss;
    for (size_t idx = 0; idx < text.length(); idx++){
        int rl = 1;
        while (idx+1 < text.length() && text[idx] == text[idx+1]){
            rl++;
            idx++;
        }
        if (rl > 1)
            ss << rl;
        ss << text[idx];
    }
    return ss.str();
}
std::string Board::rle_decode(const std::string& text) {
    std::stringstream ss;
    size_t idx = 0;
    while (idx < text.length()) {
        int digits = 0;
        while(isdigit(text[idx + digits]))
            digits++;
        int runLength = digits > 0 ? stoi(text.substr(idx, idx + digits)) : 1;
        for (int x = 0; x < runLength; x++)
            ss << text[idx + digits];
        idx += digits + 1;
    }
    return ss.str();
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
    from_string("rnbqkbnr8p32#8PRNBQKBNR");
}

std::string Board::to_string() {
    std::string result(NUM_SQUARES, '\0');
    U64 shift = 1ULL;
    for (int i = 0; i < NUM_SQUARES; i++, shift <<= 1) {
        Colors color = ALL;
        Pieces piece = EMPTY;

        for (int c = WHITE; c <= BLACK; c++) {
            for (int p = PAWN; piece <= KING; p++) {
                if (m_bitboards[c][p] & shift) {
                    piece = (Pieces)p;
                    color = (Colors)c;
                    break;
                }
            }
        }

        result[i] = get_notation(piece, color);
    }
    return rle_encode(result);
}
