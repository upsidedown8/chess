#pragma once
#include "game/board/piece/piece.hpp"

class Square {
public:
    Piece* piece;

    Square();
    Square(Piece* piece);

    void clear();
    bool isOccupied();
    Square* copy();
};