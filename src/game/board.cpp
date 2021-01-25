#include "game/board.hpp"

#include "utils.hpp"

#include <assert.h>
#include <sstream>

using namespace chess_cpp;

const std::string notation = "pnbrqkPNBRQK";

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
    U64 square = 0;
    while (square < 64 && pos < str.length()) {
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
                square += (str[pos]-'0');
                break;
            
            case '/':
            case ' ':
                break;

            default:
                return false;
        }

        if (piece != NO_PIECE) {
            bitboards[piece] |= 1ULL << (63-square);
            square++;
        }

        pos++;
    }

    // check the whole board was processed
    if (1+pos++ >= str.length() || square < 64) return false;

    // parse current move
    white_to_move = str[pos] == 'w';
    pos+=2;

    // parse castling rights
    castling = 0;
    while (pos < str.length() && str[pos] != ' ') {
        switch (str[pos++]) {
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

    // en passant
    if (str[pos] == '-') {
        en_passant = not_on_board;
        pos+=2;
    } else {
        en_passant = (7-(str[pos]-'a')) + (str[pos+1]-'0')*8;
        pos += 3;
    }

    // halfmove clock
    half_move_count = 0;
    while (isdigit(str[pos]))
        half_move_count = half_move_count*10 + (str[pos++]-'0');
    pos++;

    // fullmove count
    full_move_count = 0;
    while (isdigit(str[pos]))
        full_move_count = full_move_count*10 + (str[pos++]-'0');

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

std::string Board::to_fen() {
    std::stringstream ss;
    U64 square = 1ULL;
    int rank = 0, file = 0, empty = 0;
    while (rank < 8) {
        bool found = false;
        for (int piece = WP; piece <= BK; piece++) {
            if (bitboards[piece]&square) {
                ss << notation[piece];
                found = true;
                break;
            }
        }
        if (found) {
            if (empty)
                ss << empty;
            empty = 0;
        } else {
            empty++;
        }
        square <<= 1;
        if (++file >= 8) {
            if (empty)      ss << empty;
            if (rank++ < 7) ss << '/';
            empty = file = 0;
        }
    }

    ss << ' ' << (white_to_move ? 'w' : 'b') << ' ';

    if (castling & WHITE_CASTLE_KS) ss << 'K';
    if (castling & WHITE_CASTLE_QS) ss << 'Q';
    if (castling & BLACK_CASTLE_KS) ss << 'k';
    if (castling & BLACK_CASTLE_QS) ss << 'q';
    if (!castling)                  ss << '-';

    ss << ' ';
    if (en_passant == not_on_board) ss << '-';
    else                            ss << en_passant;
    ss << ' ' << (int)half_move_count;
    ss << ' ' << (int)full_move_count;

    return ss.str();
}
std::string Board::to_string() {
    U64 mask = 1;
    std::stringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << (8-i) << ' ';
        for (int j = 0; j < 8; j++) {
            bool found = false;
            for (int piece = WP; piece <= BK; piece++) {
                if (bitboards[piece] & mask) {
                    ss << notation[piece];
                    found = true;
                    break;
                }
            }
            if (!found)
                ss << ' ';
            ss << ' ';
            mask <<= 1;
        }
        ss << std::endl;
    }
    ss << "  a b c d e f g h";
    return ss.str();
}
