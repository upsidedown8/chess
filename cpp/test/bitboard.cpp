#include "bitboard.hpp"

BitBoard::BitBoard() {
    reset();
}

void BitBoard::reset() {
    fromString(START_POSITION);
}
void BitBoard::zeroBoards() {
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
}
void BitBoard::fromString(const std::string &board) {
    zeroBoards();
    std::string boardDecoded = RLE::decode(board);
    U64 shift = 0L;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++, shift <<= 1) {
        switch (boardDecoded[i]) {
        case W_PAWN_NOTATION:
            W_PAWNS += shift;
            break;
        case W_KNIGHT_NOTATION:
            W_KNIGHTS += shift;
            break;
        case W_BISHOP_NOTATION:
            W_BISHOPS += shift;
            break;
        case W_ROOK_NOTATION:
            W_ROOKS += shift;
            break;
        case W_QUEEN_NOTATION:
            W_QUEENS += shift;
            break;
        case W_KING_NOTATION:
            W_KINGS += shift;
            break;
            
        case B_PAWN_NOTATION:
            B_PAWNS += shift;
            break;
        case B_KNIGHT_NOTATION:
            B_KNIGHTS += shift;
            break;
        case B_BISHOP_NOTATION:
            B_BISHOPS += shift;
            break;
        case B_ROOK_NOTATION:
            B_ROOKS += shift;
            break;
        case B_QUEEN_NOTATION:
            B_QUEENS += shift;
            break;
        case B_KING_NOTATION:
            B_KINGS += shift;
            break;
        
        default:
            break;
        }
    }
}

std::string BitBoard::toString() {
    std::string result(NUMBER_OF_SQUARES, EMPTY_NOTATION);
    U64 shift = 0L;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++, shift <<= 1) {
        if      (W_PAWNS   & shift) result[i] = W_PAWN_NOTATION;
        else if (W_KNIGHTS & shift) result[i] = W_KNIGHT_NOTATION;
        else if (W_BISHOPS & shift) result[i] = W_BISHOP_NOTATION;
        else if (W_ROOKS   & shift) result[i] = W_ROOK_NOTATION;
        else if (W_QUEENS  & shift) result[i] = W_QUEEN_NOTATION;
        else if (W_KINGS   & shift) result[i] = W_KING_NOTATION;
        
        else if (B_PAWNS   & shift) result[i] = B_PAWN_NOTATION;
        else if (B_KNIGHTS & shift) result[i] = B_KNIGHT_NOTATION;
        else if (B_BISHOPS & shift) result[i] = B_BISHOP_NOTATION;
        else if (B_ROOKS   & shift) result[i] = B_ROOK_NOTATION;
        else if (B_QUEENS  & shift) result[i] = B_QUEEN_NOTATION;
        else if (B_KINGS   & shift) result[i] = B_KING_NOTATION;
    }
    return RLE::encode(result);
}

U64 BitBoard::getMask(int pos) {
    return 1L << pos;
}
U64 BitBoard::getMask(int rank, int file) {
    return 1L << (rank * 8 + file);
}

bool BitBoard::isOccupied(const U64 &board, int pos) {
    return board & getMask(pos);
}
bool BitBoard::isOccupied(const U64 &board, int rank, int file) {
    return board & getMask(rank, file);
}

void BitBoard::setPos(U64 &board, int pos, int val) {
    board &= getMask(pos);
}

int BitBoard::countOccupied(U64 board) {
    int count = 0;
    while (board) {
        board &= board-1;
        ++count;
    }
    return count;
}