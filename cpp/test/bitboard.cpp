#include "bitboard.hpp"

BitBoard::BitBoard() {

}

void BitBoard::reset() {
    W_PAWNS = 0L;
    W_KNIGHTS = 0L;
    W_BISHOPS = 0L;
    W_ROOKS = 0L;
    W_QUEENS = 0L;
    W_KINGS = 0L;

    W_PAWNS = 0L;
    W_KNIGHTS = 0L;
    W_BISHOPS = 0L;
    W_ROOKS = 0L;
    W_QUEENS = 0L;
    W_KINGS = 0L;

    fromString(*new std::string(START_POSITION));
}
void BitBoard::fromString(std::string &board) {

}