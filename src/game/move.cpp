#include "move.hpp"

using namespace chess_cpp;

Move::Move(int start, int end, U8 flags) {
    value = (end<<10) | (start<<4) | flags;
}

U8 Move::get_start() {
    return (value & MOVESTART_FLAG)>>4;
}
U8 Move::get_end() {
    return (value & MOVEEND_FLAG)>>10;
}

std::string Move::to_string() {
    std::string result(4, '\0');
    int r, f;
    calc_rf(get_start(), r, f);
    result[0] = 'a'+r;
    result[1] = '0'+f;
    calc_rf(get_end(), r, f);
    result[2] = 'a'+r;
    result[3] = '0'+f;
    return result;
}
