#include "board.hpp"

Board::Board() {
    whitePieces = std::vector<PieceType>{};
    blackPieces = std::vector<PieceType>{};
    whitePossibleMoves = std::vector<Move>{};
    blackPossibleMoves = std::vector<Move>{};
    reset();
}
Board::Board(Board* board) {
    for (int i = 0; i < 64; i++)
        this->board[i] = board->board[i];
    whitePieces = std::vector<PieceType>{};
    blackPieces = std::vector<PieceType>{};
    whitePossibleMoves = std::vector<Move>{};
    blackPossibleMoves = std::vector<Move>{};
    this->enPassantPawnVirtual = board->enPassantPawnVirtual;
    this->enPassantColour = board->enPassantColour;
    calcMoves();
}

void Board::reset() {
    enPassantPawnVirtual = -1;
    loadPosition(INITIAL_POSITION, true);
    calcMoves();
}

void Board::calcMoves() {

}
void Board::loadPosition(std::string board, bool encoded) {
    if (encoded)
        board = RLE::decode(board);
    for (int i = 0; i < 64; i++){
        char notation = board[i];
        PieceType pieceType = Piece.getPieceTypeByNotation(notation);
        assert pieceType != null;
        if (!pieceType.isEmpty()){
            PieceColor color = Character.isUpperCase(notation) ? Colour.WHITE : Colour.BLACK;
            this->board[i] = ;
        } else {
            this->board[i] = 0;
        }
    }
}
std::string toString(bool encoded) {

}

bool Board::isOnBoard(int pos) {
    return pos >= 0 && pos < 64;
}
int Board::findRank(int pos) {
    return pos / 8;
}
int Board::findFile(int pos) {
    return pos % 8;
}
int Board::getPosFromAlgebraic(std::string algebraic) {

}


