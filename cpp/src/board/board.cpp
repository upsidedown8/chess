#include "board/board.hpp"

// Board::Board() {
//     whitePieces = std::vector<PieceType>{};
//     blackPieces = std::vector<PieceType>{};
//     whitePossibleMoves = std::vector<Move>{};
//     blackPossibleMoves = std::vector<Move>{};
//     reset();
// }
// Board::Board(Board* board) {
//     for (int i = 0; i < 64; i++)
//         this->board[i] = board->board[i];
//     whitePieces = std::vector<PieceType>{};
//     blackPieces = std::vector<PieceType>{};
//     whitePossibleMoves = std::vector<Move>{};
//     blackPossibleMoves = std::vector<Move>{};
//     this->enPassantPawnVirtual = board->enPassantPawnVirtual;
//     this->enPassantColour = board->enPassantColour;
//     calcMoves();
// }

void Board::reset() {
    // enPassantPawnVirtual = -1;
    loadPosition(INITIAL_POSITION, true);
    //calcMoves();
}

// void Board::calcMoves() {

// }
void Board::loadPosition(std::string board, bool encoded) {
    if (encoded)
        board = RLE::decode(board);
    for (int i = 0; i < 64; i++){
        char notation = board[i];
        int m = isupper(notation) ? 1 : -1;
        switch (tolower(notation)) {
        case PAWN_NOTATION:
            squares[i] = static_cast<Square>(m * WPawn);
            break;
        case KNIGHT_NOTATION:
            squares[i] = static_cast<Square>(m * WKnight);
            break;
        case BISHOP_NOTATION:
            squares[i] = static_cast<Square>(m * WBishop);
            break;
        case ROOK_NOTATION:
            squares[i] = static_cast<Square>(m * WRook);
            break;
        case QUEEN_NOTATION:
            squares[i] = static_cast<Square>(m * WQueen);
            break;
        case KING_NOTATION:
            squares[i] = static_cast<Square>(m * WKing);
            break;
        default:
            squares[i] = Empty;
            break;
        }
    }
}
bool Board::isOccupied(int idx) {
    return squares[idx] != Empty;
}
bool Board::isWhite(Square piece) {
    return piece > 0;
}
int Board::getIdx(Square piece) {
    return abs(piece) - 1;
}

// std::string toString(bool encoded) {

// }

// bool Board::isOnBoard(int pos) {
//     return pos >= 0 && pos < 64;
// }
// int Board::findRank(int pos) {
//     return pos / 8;
// }
// int Board::findFile(int pos) {
//     return pos % 8;
// }
// int Board::getPosFromAlgebraic(std::string algebraic) {

// }


