#include "board.hpp"

bool Board::isOnBoard(int pos) {
    return pos >= 0 && pos < 64;
}
int Board::findRank(int pos) {
    return pos/8;
}
int Board::findFile(int pos) {
    return pos%8;
}