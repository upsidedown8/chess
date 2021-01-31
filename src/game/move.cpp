#include "move.hpp"

using namespace chess_cpp;

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
    return result;
}
