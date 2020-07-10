#include "square.hpp"

Square::Square() {
    clear();
}
Square::Square(Piece* piece) {
    this->piece = piece->copy();
    this->piece->pos = piece->pos;
}

void Square::clear() {
    piece = NULL;
}
bool Square::isOccupied() {
    return piece == NULL;
}
Square* Square::copy() {
    return isOccupied() ? new Square(piece) : new Square();
}