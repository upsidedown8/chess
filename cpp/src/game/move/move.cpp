#include "game/move/move.hpp"

bool Move::operator==(Move &other) {
    return start == other.start && end == other.end;
}