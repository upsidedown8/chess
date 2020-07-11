#include "bishop.hpp"

Piece* Bishop::copy() {
    return new Bishop(this);
}
std::vector<Move> Bishop::getPossibleMoves(Board* board) {

}