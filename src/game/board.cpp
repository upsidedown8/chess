#include "board.hpp"

#include <assert.h>
#include <sstream>

using namespace chess_cpp;

char get_notation(int piece) {
    const std::string notation = "PNBRQKpnbrqk";
    return notation[(piece & 0b111) + (piece&PIECE_COLOR ? 6 : 0)];
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
    int square = 0;
    while (square < NUM_SQUARES && pos < str.length()) {
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
                square += str[pos]-'0';
                break;
            
            case '/':
            case ' ':
                break;

            default:
                return false;
        }

        if (piece != None) {
            bitboards[piece] |= 1ULL << square;
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
            case 'q':
                castling |= BLACK_CASTLE_QS;
                break;
            case 'k':
                castling |= BLACK_CASTLE_KS;
                break;
            case 'Q':
                castling |= WHITE_CASTLE_QS;
                break;
            case 'K':
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
        en_passant = calc_pos(str[pos+1]-'1', str[pos]-'a');
        pos += 3;
    }

    // halfmove clock
    fifty_move = 0;
    while (isdigit(str[pos]))
        fifty_move = fifty_move*10 + (str[pos++]-'0');
    pos++;

    // fullmove count
    full_move_count = 0;
    while (isdigit(str[pos]))
        full_move_count = full_move_count*10 + (str[pos++]-'0');

    bitboards[White | All] = 
        bitboards[White | Pawn] |
        bitboards[White | Knight] |
        bitboards[White | Bishop] |
        bitboards[White | Rook] |
        bitboards[White | Queen] |
        bitboards[White | King];
    bitboards[Black | All] = 
        bitboards[Black | Pawn] |
        bitboards[Black | Knight] |
        bitboards[Black | Bishop] |
        bitboards[Black | Rook] |
        bitboards[Black | Queen] |
        bitboards[Black | King];

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
void Board::make_move(Move &move, UndoInfo &info) {
    U8 start, end, flag, piece;
    GET_MOVE_START(move, start);
    GET_MOVE_END(move, end);
    GET_MOVE_TYPE(move, flag);
    GET_MOVE_PIECE(move, piece);

    Colors FRIENDLY = white_to_move ? White : Black;
    Colors ENEMY = !FRIENDLY;

    info.castling = castling;
    info.fifty_move = fifty_move;
    info.en_passant = en_passant;
    info.captured = pieces[end];

    assert(pieces[start] != None);
    assert(start != end);
    assert((pieces[start]&PIECE_COLOR) == FRIENDLY);
    assert(pieces[end] == None || (pieces[start]&PIECE_COLOR) != (pieces[end]&PIECE_COLOR));

    if ((pieces[start]&0b111)==Pawn || pieces[end] != None) {
        fifty_move = 0;
    } else {
        fifty_move++;
    }

    switch (flag) {
        case MOVETYPE_ENPASSANT: {
            assert(en_passant != not_on_board);

            clr_pos(bitboards[pieces[end]], end);
            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[pieces[start]], en_passant);
            clr_pos(bitboards[ENEMY|All], end);
            clr_pos(bitboards[FRIENDLY|All], start);
            set_pos(bitboards[FRIENDLY|All], en_passant);

            pieces[en_passant] = pieces[start];
            pieces[start] = None;
            pieces[end] = None;

            en_passant = not_on_board;

            break;
        }
        case MOVETYPE_CASTLE: {
            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[pieces[start]], end);
            clr_pos(bitboards[FRIENDLY|All], start);
            set_pos(bitboards[FRIENDLY|All], end);

            int offset = start & 0b111000;
            switch (piece) {
                case MOVECASTLE_QS: {
                    assert(castling & (0b01<<(2*white_to_move)));

                    set_pos(bitboards[pieces[offset]], offset+3);
                    clr_pos(bitboards[pieces[offset]], offset);
                    set_pos(bitboards[FRIENDLY|All], offset+3);
                    clr_pos(bitboards[FRIENDLY|All], offset);

                    pieces[offset+3] = pieces[offset];
                    pieces[offset] = None;
                    break;
                }
                case MOVECASTLE_KS:
                default: {
                    assert(castling & (0b10<<(2*white_to_move)));

                    set_pos(bitboards[pieces[offset+7]], offset+5);
                    clr_pos(bitboards[pieces[offset+7]], offset+7);
                    set_pos(bitboards[FRIENDLY|All], offset+5);
                    clr_pos(bitboards[FRIENDLY|All], offset+7);

                    pieces[offset+5] = pieces[offset+7];
                    pieces[offset+7] = None;
                    break;
                }
            }
            castling &= 0b1100>>(2*white_to_move);

            pieces[end] = pieces[start];
            pieces[start] = None;
            
            en_passant = not_on_board;
            break;
        }
        case MOVETYPE_PROMOTION: {
            int promotionPiece = FRIENDLY | (piece+1);
            if (pieces[end] != None) {
                clr_pos(bitboards[pieces[end]], end);
                clr_pos(bitboards[ENEMY|All], end);
            }

            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[promotionPiece], end);
            clr_pos(bitboards[FRIENDLY|All], start);
            set_pos(bitboards[FRIENDLY|All], end);

            if (pieces[end] == (ENEMY|Rook)) {
                // if FRIENDLY PAWN takes ENEMY ROOK, then that enemy side cannot be castled to
                switch (end) {
                    case a1: castling &= ~WHITE_CASTLE_QS; break;
                    case h1: castling &= ~WHITE_CASTLE_KS; break;
                    case a8: castling &= ~BLACK_CASTLE_QS; break;
                    case h8: castling &= ~BLACK_CASTLE_KS; break;
                    default: break;
                }
            }

            pieces[start] = None;
            pieces[end] = promotionPiece;
            en_passant = not_on_board;
            break;
        }
        // capture/simple
        default: {
            en_passant = ((pieces[start]&0b111) == Pawn && abs(end-start) == 16)
                ? ((start+end)>>1)
                : not_on_board;

            if (pieces[end] != None) {
                clr_pos(bitboards[pieces[end]], end);
                clr_pos(bitboards[ENEMY|All], end);
            }

            switch (pieces[start] & 0b111) {
                case King: {
                    // if the king moves then that player cannot castle
                    castling &= 0b1100>>(2*white_to_move);
                }
                case Rook: {
                    // if FRIENDLY ROOK moved FROM square, then that side cannot be castled to
                    switch (start) {
                        case a1: castling &= ~WHITE_CASTLE_QS; break;
                        case h1: castling &= ~WHITE_CASTLE_KS; break;
                        case a8: castling &= ~BLACK_CASTLE_QS; break;
                        case h8: castling &= ~BLACK_CASTLE_KS; break;
                        default: break;
                    }
                }
                default: break;
            }

            if (pieces[end] == (enemy_color()|Rook)) {
                // if ANY FRIENDLY PIECE takes ENEMY ROOK, then that enemy side cannot be castled to
                switch (end) {
                    case a1: castling &= ~WHITE_CASTLE_QS; break;
                    case h1: castling &= ~WHITE_CASTLE_KS; break;
                    case a8: castling &= ~BLACK_CASTLE_QS; break;
                    case h8: castling &= ~BLACK_CASTLE_KS; break;
                    default: break;
                }
            }

            clr_pos(bitboards[pieces[start]], start);
            set_pos(bitboards[pieces[start]], end);
            clr_pos(bitboards[FRIENDLY|All], start);
            set_pos(bitboards[FRIENDLY|All], end);

            pieces[end] = pieces[start];
            pieces[start] = None;
            break;
        }
    }

    white_to_move = !white_to_move;
}
void Board::undo_move(Move &move, UndoInfo &info) {
    U8 start, end, flag, piece;
    GET_MOVE_START(move, start);
    GET_MOVE_END(move, end);
    GET_MOVE_TYPE(move, flag);
    GET_MOVE_PIECE(move, piece);

    white_to_move = !white_to_move;
    castling = info.castling;
    fifty_move = info.fifty_move;
    en_passant = info.en_passant;
    int endPiece = info.captured;

    Colors FRIENDLY = white_to_move ? White : Black;
    Colors ENEMY = !FRIENDLY;

    assert(start != end);
    assert(pieces[start] == None);
    if (flag == MOVETYPE_ENPASSANT) {
        assert(pieces[end] == None);
        assert((pieces[en_passant]&PIECE_COLOR) == FRIENDLY);
    } else {
        assert(pieces[end] != None);
        assert((pieces[end]&PIECE_COLOR) == FRIENDLY);
    }

    switch (flag) {
        case MOVETYPE_ENPASSANT: {
            assert(en_passant != not_on_board);

            set_pos(bitboards[ENEMY | Pawn], end);
            set_pos(bitboards[FRIENDLY | Pawn], start);
            clr_pos(bitboards[FRIENDLY | Pawn], en_passant);
            set_pos(bitboards[ENEMY | All], end);
            set_pos(bitboards[FRIENDLY | All], start);
            clr_pos(bitboards[FRIENDLY | All], en_passant);

            pieces[en_passant] = None;
            pieces[start] = FRIENDLY | Pawn;
            pieces[end] = ENEMY | Pawn;

            break;
        }
        case MOVETYPE_CASTLE: {
            set_pos(bitboards[FRIENDLY | King], start);
            clr_pos(bitboards[FRIENDLY | King], end);
            set_pos(bitboards[FRIENDLY | All], start);
            clr_pos(bitboards[FRIENDLY | All], end);

            pieces[start] = FRIENDLY | King;
            pieces[end] = None;

            int offset = start & 0b111000;
            switch (piece) {
                case MOVECASTLE_QS: {
                    clr_pos(bitboards[FRIENDLY | Rook], offset+3);
                    set_pos(bitboards[FRIENDLY | Rook], offset);
                    clr_pos(bitboards[FRIENDLY | All], offset+3);
                    set_pos(bitboards[FRIENDLY | All], offset);

                    pieces[offset] = FRIENDLY | Rook;
                    pieces[offset+3] = None;
                    break;
                }
                case MOVECASTLE_KS:
                default: {
                    clr_pos(bitboards[FRIENDLY | Rook], offset+5);
                    set_pos(bitboards[FRIENDLY | Rook], offset+7);
                    clr_pos(bitboards[FRIENDLY | All], offset+5);
                    set_pos(bitboards[FRIENDLY | All], offset+7);

                    pieces[offset+7] = FRIENDLY | Rook;
                    pieces[offset+5] = None;
                    break;
                }
            }
            break;
        }
        case MOVETYPE_PROMOTION: {
            int promotionPiece = FRIENDLY | (piece+1);

            set_pos(bitboards[FRIENDLY | Pawn], start);
            clr_pos(bitboards[promotionPiece], end);
            set_pos(bitboards[FRIENDLY | All], start);
            clr_pos(bitboards[FRIENDLY | All], end);

            pieces[start] = FRIENDLY | Pawn;
            pieces[end] = endPiece;

            if (endPiece != None) {
                set_pos(bitboards[endPiece], end);
                set_pos(bitboards[ENEMY | All], end);
            }

            break;
        }
        // capture/simple
        default: {
            set_pos(bitboards[pieces[end]], start);
            clr_pos(bitboards[pieces[end]], end);
            set_pos(bitboards[FRIENDLY | All], start);
            clr_pos(bitboards[FRIENDLY | All], end);
            pieces[start] = pieces[end];
            pieces[end] = endPiece;

            if (endPiece != None) {
                set_pos(bitboards[endPiece], end);
                set_pos(bitboards[ENEMY | All], end);
            }

            break;
        }
    }
}

Colors Board::active_color() {
    return white_to_move ? White : Black;
}
Colors Board::enemy_color() {
    return white_to_move ? Black : White;
}

std::string Board::to_fen() {
    std::stringstream ss;
    int square = 0;
    int rank = 0, file = 0, empty = 0;
    while (rank < 8) {
        assert(0 <= square && square <= NUM_SQUARES);
        if (pieces[square] != None) {
            if (empty)
                ss << empty;
            empty = 0;
            ss << get_notation(pieces[square]);
        } else {
            empty++;
        }
        square++;
        if (++file >= 8) {
            if (empty)      ss << empty;
            if (rank++ < 7) ss << '/';
            empty = file = 0;
        }
    }

    ss << ' ' << (white_to_move ? 'w' : 'b') << ' ';

    if (castling & WHITE_CASTLE_QS) ss << 'Q';
    if (castling & BLACK_CASTLE_QS) ss << 'q';
    if (castling & WHITE_CASTLE_KS) ss << 'K';
    if (castling & BLACK_CASTLE_KS) ss << 'k';
    if (!castling)                  ss << '-';

    ss << ' ';
    if (en_passant == not_on_board) ss << '-';
    else {
        int r, f;
        calc_rf(en_passant, r, f);
        ss << (char)('a' + f) << (char)('1' + r);
    }
    ss << ' ' << (int)fifty_move;
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
    ss << "  a b c d e f g h" << std::endl;
    ss << (white_to_move ? "White" : "Black") << " to move";
    return ss.str();
}

bool Board::operator==(Board &other) {
    for (int color : {White, Black}) {
        for (int piece = Pawn; piece <= All; piece++) {
            if (bitboards[color|piece] != other.bitboards[color|piece]) {
                return false;
            }
        }
    }
    for (int sq = 0; sq < NUM_SQUARES; sq++) {
        if (pieces[sq] != other.pieces[sq]) {
            return false;
        }
    }
    return 
        fifty_move == other.fifty_move &&
        castling == other.castling &&
        en_passant == other.en_passant &&
        full_move_count == other.full_move_count &&
        white_to_move == other.white_to_move;
}
