#include "move.hpp"

using namespace chess_cpp;

Move::Move()
    : value(0) {}
Move::Move(int start, int end, U8 flags) {
    value = (end<<10) | (start<<4) | flags;
}

U8 Move::get_start() {
    return (value & MOVEFLAG_START)>>4;
}
U8 Move::get_end() {
    return (value & MOVEFLAG_END)>>10;
}

std::string Move::to_string() {
    std::string result(4, '\0');
    int r, f;
    calc_rf(get_start(), r, f);
    result[0] = 'a'+f;
    result[1] = '1'+r;
    calc_rf(get_end(), r, f);
    result[2] = 'a'+f;
    if ((value & MOVEFLAG_TYPE) == MOVETYPE_ENPASSANT) {
        result[3] = '1'+(r==3?2:5);
    } else {
        result[3] = '1'+r;
    }
    if ((value & MOVEFLAG_TYPE) == MOVETYPE_PROMOTION) {
        result += "NBRQ"[value & MOVEFLAG_PIECE];
    }
    return result;
}

UndoInfo::UndoInfo(int fiftyMove, int enPassant, int piece) {
    fifty_move = fiftyMove;
    en_passant = enPassant;
    captured = piece;
}

U8 UndoInfo::get_fifty_move() {
    return fifty_move;
}
U8 UndoInfo::get_en_passant() {
    return en_passant;
}
int UndoInfo::get_captured_piece() {
    return captured;
}
