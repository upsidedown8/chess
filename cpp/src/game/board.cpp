#include "game/board.hpp"

Board::Board() {
    reset();
}

void Board::reset() {
    // enPassantPawnVirtual = -1;
    loadPosition(INITIAL_POSITION, true);
    //calcMoves();
}

Square& Board::operator[](int idx) {
    return squareAt(idx);
}
Square& Board::squareAt(int idx) {
    return squares[idx];
}

void Board::calcMoves() {
    whiteMoves.clear();
    blackMoves.clear();
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        if (isOccupied(i)) {
            Square piece = squareAt(i);
            bool white = isWhite(piece);
            switch (piece) {
            case WPawn:
            case BPawn:
                findPawnMoves(i, white);
                break;
            case WKnight:
            case BKnight:
                findKnightMoves(i, white);
                break;
            case WBishop:
            case BBishop:
                findBishopMoves(i, white);
                break;
            case WRook:
            case BRook:
                findRookMoves(i, white);
                break;
            case WQueen:
            case BQueen:
                findQueenMoves(i, white);
                break;
            case WKing:
            case BKing:
                findKingMoves(i, white);
                break;
            case Empty:
            default:
                break;
            }
        }
    }
}
void Board::loadPosition(string board, bool encoded) {
    if (encoded)
        board = RLE::decode(board);
    for (int i = 0; i < 64; i++){
        char notation = board[i];
        int m = isupper(notation) ? 1 : -1;
        Square result;
        switch (tolower(notation)) {
        case PAWN_NOTATION:
            result = static_cast<Square>(m * WPawn);
            break;
        case KNIGHT_NOTATION:
            result = static_cast<Square>(m * WKnight);
            break;
        case BISHOP_NOTATION:
            result = static_cast<Square>(m * WBishop);
            break;
        case ROOK_NOTATION:
            result = static_cast<Square>(m * WRook);
            break;
        case QUEEN_NOTATION:
            result = static_cast<Square>(m * WQueen);
            break;
        case KING_NOTATION:
            result = static_cast<Square>(m * WKing);
            break;
        default:
            result = Empty;
            break;
        }
        squareAt(i) = result;
    }
}
void Board::emptySquare(int idx) {
    squareAt(idx) = Empty;
}
bool Board::isOccupied(int idx) {
    return squareAt(idx) != Empty;
}
bool Board::isWhite(Square piece) {
    return piece > 0;
}
int Board::getIdx(Square piece) {
    return abs(piece) - 1;
}

string Board::toString(bool encoded) {
    string result(NUMBER_OF_SQUARES, EMPTY_NOTATION);
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        if (isOccupied(squareAt(i))) {
            bool white = isWhite(squareAt(i));
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

vector<Move> Board::getPossibleMoves(int idx) {
    vector<Move> moves;
    if (isOccupied(idx)) {
        Square square = squareAt(idx);
        vector<Move> *playerMoves = isWhite(square) ? &whiteMoves : &blackMoves;
        for (size_t i = 0; i < playerMoves->size(); i++) {
            Move move = playerMoves->at(i);
            if (move.start == idx) {
                moves.push_back(move);
            }
        }
    }
    return moves;
}
void Board::findPawnMoves(int idx, bool white) {
    int rank = findRank(idx);
    // move 1 square forward
    if (rank != 0 && rank != 7){
        int multiplier = white ? -1 : 1;
        if (!isOccupied(idx + multiplier * 8)){
            // move 2 squares forward
            int newPosition = idx + multiplier * 16;
            if (Board::isOnBoard(newPosition)){
                if (!isOccupied(newPosition) && rank == (white ? 6 : 1)){
                    possibleMoves.add(new Simple(board, idx, idx + multiplier * 16));
                }
            }
            if (rank == (white ? 1 : 6))
                possibleMoves.add(new Promotion(board, idx, idx + multiplier * 8));
            else
                possibleMoves.add(new Simple(board, idx, idx + multiplier * 8));
        }
        int file = MoveUtils.findFile(idx);
        for (int possibleMove : POSSIBLE_MOVES){
            int possiblePosition = idx + multiplier * possibleMove;
            if (!((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                    (file == 7 && (possibleMove == -7 || possibleMove == 9)))) {
                Square square = board.getSquare(possiblePosition);
                //normal capturing
                if (square.isOccupied()) {
                    if (square.piece.colour != colour) {
                        if (rank == (this.colour.isWhite() ? 1 : 6))
                            possibleMoves.add(new Promotion(board, idx, possiblePosition));
                        else
                            possibleMoves.add(new Capture(board, idx, possiblePosition));
                    }
                }

                //en passant
                else if (board.enPassantPawnVirtual == possiblePosition){
                    int actualPawnPos = board.enPassantPawnVirtual + (board.enPassantColour.isWhite() ? -8 : 8);
                    if (board.getSquare(actualPawnPos).piece.colour != colour) {
                        possibleMoves.add(new EnPassant(board, idx, actualPawnPos));
                    }
                }
            }
        }
    }
}
void Board::findKnightMoves(int idx, bool white) {
    
}
void Board::findBishopMoves(int idx, bool white) {
    
}
void Board::findRookMoves(int idx, bool white) {
    
}
void Board::findQueenMoves(int idx, bool white) {
    
}
void Board::findKingMoves(int idx, bool white) {
    
}

bool Board::isOnBoard(int pos) {
    return pos >= 0 && pos < NUMBER_OF_SQUARES;
}
int Board::findRank(int pos) {
    return pos / 8;
}
int Board::findFile(int pos) {
    return pos % 8;
}

// int Board::getPosFromAlgebraic(std::string algebraic) {

// }


