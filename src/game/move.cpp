#include "move.hpp"

using namespace chess_cpp;

/* -------------------------------------------------------------------------- */
/*                                    Move                                    */
/* -------------------------------------------------------------------------- */
std::string chess_cpp::move_to_string(Move &move) {
    std::string result(4, '\0');
    int start, end, r, f, flag;

    GET_MOVE_START(move, start);
    GET_MOVE_END(move, end);
    
    calc_rf(start, r, f);
    result[0] = 'a'+f;
    result[1] = '1'+r;
    
    calc_rf(end, r, f);
    result[2] = 'a'+f;

    GET_MOVE_TYPE(move, flag);
    if (flag == MOVETYPE_ENPASSANT) {
        result[3] = '1'+(r==3?2:5);
    } else {
        result[3] = '1'+r;
    }
    if (flag == MOVETYPE_PROMOTION) {
        int piece;
        GET_MOVE_PIECE(move, piece);
        result += "nbrq"[piece];
    }
    return result;
}
