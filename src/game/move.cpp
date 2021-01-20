#include "move.hpp"

using namespace chess_cpp;

U8 Move::get_start() {
    return start;
}
U8 Move::get_end() {
    return end;
}

Move::Move(U8 start, U8 end)
    : start(start), end(end) {}
