#include "game/bitboard.hpp"

void BitBoardUtils::zeroBoards(BitBoard &board) {
    board.W_PAWNS   = 0ULL;
    board.W_KNIGHTS = 0ULL;
    board.W_BISHOPS = 0ULL;
    board.W_ROOKS   = 0ULL;
    board.W_QUEENS  = 0ULL;
    board.W_KINGS   = 0ULL;

    board.W_PAWNS   = 0ULL;
    board.W_KNIGHTS = 0ULL;
    board.W_BISHOPS = 0ULL;
    board.W_ROOKS   = 0ULL;
    board.W_QUEENS  = 0ULL;
    board.W_KINGS   = 0ULL;
}
void BitBoardUtils::bbFromString(BitBoard &result, const std::string &board) {
    zeroBoards(result);
    std::string boardDecoded = RLE::decode(board);
    U64 shift = 1ULL;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++, shift <<= 1) {
        switch (boardDecoded[i]) {
        case W_PAWN_NOTATION:
            result.W_PAWNS |= shift;
            break;
        case W_KNIGHT_NOTATION:
            result.W_KNIGHTS |= shift;
            break;
        case W_BISHOP_NOTATION:
            result.W_BISHOPS |= shift;
            break;
        case W_ROOK_NOTATION:
            result.W_ROOKS |= shift;
            break;
        case W_QUEEN_NOTATION:
            result.W_QUEENS |= shift;
            break;
        case W_KING_NOTATION:
            result.W_KINGS |= shift;
            break;
            
        case B_PAWN_NOTATION:
            result.B_PAWNS |= shift;
            break;
        case B_KNIGHT_NOTATION:
            result.B_KNIGHTS |= shift;
            break;
        case B_BISHOP_NOTATION:
            result.B_BISHOPS |= shift;
            break;
        case B_ROOK_NOTATION:
            result.B_ROOKS |= shift;
            break;
        case B_QUEEN_NOTATION:
            result.B_QUEENS |= shift;
            break;
        case B_KING_NOTATION:
            result.B_KINGS |= shift;
            break;
        
        default:
            break;
        }
    }
}

std::string BitBoardUtils::bbToString(BitBoard &board) {
    std::string result(NUMBER_OF_SQUARES, EMPTY_NOTATION);
    U64 shift = 1ULL;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++, shift <<= 1) {
        if      (board.W_PAWNS   & shift) result[i] = W_PAWN_NOTATION;
        else if (board.W_KNIGHTS & shift) result[i] = W_KNIGHT_NOTATION;
        else if (board.W_BISHOPS & shift) result[i] = W_BISHOP_NOTATION;
        else if (board.W_ROOKS   & shift) result[i] = W_ROOK_NOTATION;
        else if (board.W_QUEENS  & shift) result[i] = W_QUEEN_NOTATION;
        else if (board.W_KINGS   & shift) result[i] = W_KING_NOTATION;
        
        else if (board.B_PAWNS   & shift) result[i] = B_PAWN_NOTATION;
        else if (board.B_KNIGHTS & shift) result[i] = B_KNIGHT_NOTATION;
        else if (board.B_BISHOPS & shift) result[i] = B_BISHOP_NOTATION;
        else if (board.B_ROOKS   & shift) result[i] = B_ROOK_NOTATION;
        else if (board.B_QUEENS  & shift) result[i] = B_QUEEN_NOTATION;
        else if (board.B_KINGS   & shift) result[i] = B_KING_NOTATION;
    }
    return RLE::encode(result);
}

bool BitBoardUtils::isSet(const U64 &board, uchar pos) {
    return board & SET_BIT_TABLE[pos];
}

void BitBoardUtils::setPos(U64 &board, uchar pos) {
    board |= SET_BIT_TABLE[pos];
}
void BitBoardUtils::clrPos(U64 &board, uchar pos) {
    board &= CLR_BIT_TABLE[pos];
}

uchar BitBoardUtils::countOccupied(U64 board) {
    uchar count = 0;
    while (board) {
        board &= board-1;
        ++count;
    }
    return count;
}
uchar BitBoardUtils::popLSB(U64 &board) {
    assert(board != 0);
    U64 b = board ^ (board - 1);
    unsigned int folded = (unsigned)((b & 0xffffffff) ^ (board >> 32));
    board &= (board - 1);
    return LSB_64_TABLE[(folded * 0x78291ACF) >> 26];
}
