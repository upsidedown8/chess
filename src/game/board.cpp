#include "game/board.hpp"

Board::Board() {
    bitboard = *new BitBoard;
    currentInfo = *new CompleteMove;
    reset();
}

void Board::reset() {
    BitBoardUtils::bbFromString(bitboard, START_POSITION);
    updateHelpers();
}

std::string Board::toString() {
    return BitBoardUtils::bbToString(bitboard);
}

CompleteMove Board::getMoveInfo(Move &move) {
    
}

bool Board::doMove(const CompleteMove &moveInfo) {
    updateHelpers();
    return true;
}
bool Board::undoMove(const CompleteMove &moveInfo) {
    updateHelpers();
    return true;
}

void Board::updateHelpers() {
    W_PIECES =
        bitboard.W_PAWNS |
        bitboard.W_KNIGHTS |
        bitboard.W_BISHOPS |
        bitboard.W_ROOKS |
        bitboard.W_QUEENS |
        bitboard.W_KINGS;

    B_PIECES =
        bitboard.B_PAWNS |
        bitboard.B_KNIGHTS |
        bitboard.B_BISHOPS |
        bitboard.B_ROOKS |
        bitboard.B_QUEENS |
        bitboard.B_KINGS;

    PIECES = W_PIECES | B_PIECES;
}

