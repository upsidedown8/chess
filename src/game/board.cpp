#include "game/board.hpp"

#include "utils.hpp"

#include <assert.h>
#include <sstream>

using namespace chess_cpp;

void Board::zero_boards() {
    for (int piece = WP; piece <= BK; piece++)
        bitboards[piece] = 0ULL;
}
void Board::update_bitboards() {
    for (int piece = WP; piece <= WK; piece++)
        combined[piece] =
            bitboards[piece] |
            bitboards[piece+BP];
}
bool Board::from_string(const std::string &str) {
    zero_boards();

    // parse board
    int pos = 0;
    U64 square = 1ULL;
    while (square && pos < str.length()) {
        int piece = NO_PIECE;
        switch (str[pos]) {
            case 'p': piece = BP; break;
            case 'n': piece = BN; break;
            case 'b': piece = BB; break;
            case 'r': piece = BR; break;
            case 'q': piece = BQ; break;
            case 'k': piece = BK; break;

            case 'P': piece = WP; break;
            case 'N': piece = WN; break;
            case 'B': piece = WB; break;
            case 'R': piece = WR; break;
            case 'Q': piece = WQ; break;
            case 'K': piece = WK; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                square <<= (str[pos]-'0');
                break;
            
            case '/':
            case ' ':
                break;

            default:
                return false;
        }

        if (piece != NO_PIECE) {
            bitboards[piece] |= square;
            square <<= 1;
        }
    }

    // check the whole board was processed
    if (1+pos++ >= str.length() || square) return false;

    // parse current move
    white_to_move = str[pos] == 'w';
    pos+=2;

    // parse castling rights
    castling = 0;
    while (pos < str.length() && str[pos] != ' ') {
        switch (str[pos]) {
            case 'Q':
                castling |= BLACK_CASTLE_QS;
                break;
            case 'K':
                castling |= BLACK_CASTLE_KS;
                break;
            case 'q':
                castling |= WHITE_CASTLE_QS;
                break;
            case 'k':
                castling |= WHITE_CASTLE_KS;
                break;
            case '-':
                break;
            default:
                return false;
        }
    }
    pos++;

    

    update_bitboards();

    return true;
}

Board::Board() {
    reset();
}
Board::Board(const std::string &str) {
    bool fen_success = from_string(str);
    assert(fen_success);
}

void Board::reset() {
    bool fen_success = from_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    assert(fen_success);
}
