#include "game/board.hpp"

#include <assert.h>
#include <sstream>

using namespace chess_cpp;

char get_notation(int piece) {
    const std::string notation = "pnbrqkPNBRQK";
    return notation[(piece & 0b111) + (piece&Black ? 6 : 0)];
}

void Board::zero_boards() {
    for (int color : {White, Black})
        for (int piece = Pawn; piece <= None; piece++)
            bitboards[color | piece] = 0ULL;
    for (int i = 0; i < NUM_SQUARES; i++)
        pieces[i] = None;
}
bool Board::from_string(const std::string &str) {
    zero_boards();

    // parse board
    int pos = 0;
    U64 square = 0;
    while (square < 64 && pos < str.length()) {
        int piece = None;
        switch (str[pos]) {
            case 'p': piece = Black | Pawn;   break;
            case 'n': piece = Black | Knight; break;
            case 'b': piece = Black | Bishop; break;
            case 'r': piece = Black | Rook;   break;
            case 'q': piece = Black | Queen;  break;
            case 'k': piece = Black | King;   break;

            case 'P': piece = White | Pawn;   break;
            case 'N': piece = White | Knight; break;
            case 'B': piece = White | Bishop; break;
            case 'R': piece = White | Rook;   break;
            case 'Q': piece = White | Queen;  break;
            case 'K': piece = White | King;   break;

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

        if (piece != None) {
            bitboards[piece] |= 1ULL << (63-square);
            pieces[square] = piece;
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
void Board::make_move(Move &move) {
    U8 start = move.get_start();
    U8 end   = move.get_end();

    assert(pieces[start] != None);
    assert(start != end);
    assert((pieces[start]&PIECE_COLOR) == (white_to_move?White:Black));
    assert((pieces[end] == None) || pieces[start]&PIECE_COLOR != pieces[end]&PIECE_COLOR);

    switch (move.value & MOVEFLAG_TYPE) {
        case MOVETYPE_ENPASSANT: {
            assert(en_passant != not_on_board);
            set_pos(bitboards[pieces[start]], en_passant);
            clr_pos(bitboards[pieces[start]], start);
            clr_pos(bitboards[pieces[end]], end);
            pieces[en_passant] = start;
            pieces[start] = None;
            pieces[end] = None;
            en_passant = not_on_board;
            break;
        }
        case MOVETYPE_CASTLE: {
            set_pos(bitboards[pieces[start]], end);
            clr_pos(bitboards[pieces[start]], start);
            pieces[end] = pieces[start];
            pieces[start] = None;
            int offset = start - (start%8);
            switch (move.value&MOVEFLAG_PIECE) {
                case MOVECASTLE_QS: {
                    assert(castling & (0b01<<(2*white_to_move)));
                    castling &= ~(0b01<<(2*white_to_move));
                    set_pos(bitboards[pieces[offset]], offset+3);
                    clr_pos(bitboards[pieces[offset]], offset);
                    pieces[offset+3] = pieces[offset];
                    pieces[offset] = None;
                    break;
                }
                case MOVECASTLE_KS:
                default: {
                    assert(castling & (0b10<<(2*white_to_move)));
                    castling &= ~(0b10<<(2*white_to_move));
                    set_pos(bitboards[pieces[offset+7]], offset+5);
                    clr_pos(bitboards[pieces[offset+7]], offset+7);
                    pieces[offset+5] = pieces[offset+7];
                    pieces[offset+7] = None;
                    break;
                }
            }
            en_passant = not_on_board;
            break;
        }
        case MOVETYPE_PROMOTION: {
            int promotionPiece = (pieces[start]&PIECE_COLOR) | (1+(move.value & MOVEFLAG_PIECE));
            if (pieces[end] != None)
                clr_pos(bitboards[pieces[end]], end);
            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[promotionPiece], end);
            pieces[start] = None;
            pieces[end] = promotionPiece;
            en_passant = not_on_board;
            break;
        }
        // capture/simple
        default: {
            if ((pieces[start]&0b111) == Pawn && abs(end-start) == 16)
                en_passant = (start+end)>>1;
            else
                en_passant = not_on_board;
            
            if (pieces[end] != None)
                clr_pos(bitboards[pieces[end]], end);
            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[pieces[start]], end);
            pieces[end] = pieces[start];
            pieces[start] = None;
            break;
        }
    }

    white_to_move = !white_to_move;
}

std::string Board::to_fen() {
    std::stringstream ss;
    U64 square = 1ULL;
    int rank = 0, file = 0, empty = 0;
    while (rank < 8) {
        bool found = false;
        for (int color : {White, Black}) {
            for (int piece = Pawn; piece <= King; piece++) {
                if (bitboards[color | piece] & square) {
                    ss << get_notation(color | piece);
                    found = true;
                    break;
                }
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
    std::stringstream ss;
    int pos = 0;
    for (int i = 0; i < 8; i++) {
        ss << (8-i) << ' ';
        for (int j = 0; j < 8; j++) {
            ss << (pieces[pos]==None?' ':get_notation(pieces[pos])) << ' ';
            pos++;
        }
        ss << std::endl;
    }
    ss << "  a b c d e f g h";
    return ss.str();
}
