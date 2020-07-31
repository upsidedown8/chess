#include "game/board.hpp"

Board::Board() {
    reset();
}

void Board::reset() {
    loadPosition(INITIAL_POSITION, true);
}

Square& Board::operator [](const int idx) {
    return squareAt(idx);
}
Square& Board::squareAt(const int idx) {
    return squares[idx];
}

void Board::loadPosition(string board, bool encoded) {
    if (encoded)
        board = RLE::decode(board);
    for (int i = 0; i < 64; i++){
        char notation = board[i];
        bool white = isupper(notation);
        Square result;
        switch (tolower(notation)) {
        case PAWN_NOTATION:
            result = getPiece(WPawn, white);
            break;
        case KNIGHT_NOTATION:
            result = getPiece(WKnight, white);
            break;
        case BISHOP_NOTATION:
            result = getPiece(WBishop, white);
            break;
        case ROOK_NOTATION:
            result = getPiece(WRook, white);
            break;
        case QUEEN_NOTATION:
            result = getPiece(WQueen, white);
            break;
        case KING_NOTATION:
            result = getPiece(WKing, white);
            break;
        default:
            result = Empty;
            break;
        }
        squareAt(i) = result;
    }
}
void Board::clearSquare(int idx) {
    squareAt(idx) = Empty;
}
bool Board::isOccupied(int idx) {
    return isOccupied(squareAt(idx));
}
int Board::getIdx(Square piece) {
    return abs(piece) - 1;
}

string Board::toString(bool encoded) {
    string result(NUMBER_OF_SQUARES, EMPTY_NOTATION);
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        Square square = squareAt(i);
        if (isOccupied(square)) {
            bool white = isWhite(square);
            char notation;
            switch (result[i]) {
                case WPawn:
                case BPawn:
                    notation = PAWN_NOTATION;
                    break;
                case WKnight:
                case BKnight:
                    notation = KNIGHT_NOTATION;
                    break;
                case WBishop:
                case BBishop:
                    notation = BISHOP_NOTATION;
                    break;
                case WRook:
                case BRook:
                    notation = ROOK_NOTATION;
                    break;
                case WQueen:
                case BQueen:
                    notation = QUEEN_NOTATION;
                    break;
                case WKing:
                case BKing:
                    notation = KING_NOTATION;
                    break;
                default:
                    notation = EMPTY_NOTATION;
                    break;
            }
            result[i] = white ? toupper(notation) : tolower(notation);
        }
    }
    return encoded ? RLE::encode(result) : result;
}

int Board::findRank(int pos) {
    return pos / 8;
}
int Board::findFile(int pos) {
    return pos % 8;
}
bool Board::isOnBoard(int pos) {
    return pos >= 0 && pos < NUMBER_OF_SQUARES;
}
bool Board::isWhite(Square piece) {
    return piece > 0;
}
bool Board::isOccupied(Square square) {
    return square != Empty;
}
Square Board::swapColour(Square piece) {
    return static_cast<Square>(-piece);
}
Square Board::getPiece(Square piece, bool white) {
    return static_cast<Square>(abs(piece) * (white ? 1 : -1));
}
