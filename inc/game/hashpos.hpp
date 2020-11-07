#pragma once
#include "board.hpp"

// Zobrist hashing
class HashPos {
private:
    U64 randTable[64][12];

public:
    HashPos();

    U64 hash(Board &board);
};