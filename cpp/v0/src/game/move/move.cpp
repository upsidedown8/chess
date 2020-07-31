#include "game/move/move.hpp"

bool Move::operator==(Move &other) {
    return start == other.start && end == other.end;
}

void Move::init(Board &board, int start, int end) {
    this->start = start;
    this->end = end;
    this->startPiece = board[start];
}