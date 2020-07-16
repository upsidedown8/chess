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
    int whiteKingPos = 0, blackKingPos = 0;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        Square piece = squareAt(i);
        if (isOccupied(piece)) {
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
                whiteKingPos = i;
                break;
            case BKing:
                blackKingPos = i;
                break;
            case Empty:
            default:
                break;
            }
        }
    }
    for (size_t i = 0; i < NUMBER_OF_SQUARES; i++) {
        whiteDestinations[i] = false;
        blackDestinations[i] = false;
    }
    for (size_t i = 0; i < whiteMoves.size(); i++)
        whiteDestinations[whiteMoves.at(i).end] = true;
    for (size_t i = 0; i < blackMoves.size(); i++)
        blackDestinations[blackMoves.at(i).end] = true;
    findKingMoves(whiteKingPos, true);
    findKingMoves(blackKingPos, false);
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
void Board::clearSquare(int idx) {
    squareAt(idx) = Empty;
}
bool Board::isOccupied(int idx) {
    return isOccupied(squareAt(idx));
}
bool Board::isOccupied(Square square) {
    return square != Empty;
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

vector<Move> Board::getPossibleMoves(int idx) {
    vector<Move> moves;
    Square square = squareAt(idx);
    if (isOccupied(square)) {
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
    int rank = Board::findRank(idx);
    // move 1 square forward
    if (rank != 0 && rank != 7){
        int multiplier = white ? -1 : 1;
        int singleForwardMove = idx + multiplier * 8;
        if (!isOccupied(singleForwardMove)){
            // move 2 squares forward
            int doubleForwardMove = singleForwardMove + multiplier * 8;
            if (Board::isOnBoard(doubleForwardMove))
                if (!isOccupied(doubleForwardMove) && rank == (white ? 6 : 1))
                    addMove(white, *new SimpleMove(*this, idx, doubleForwardMove));
            if (rank == (white ? 1 : 6)) {
                addMove(white, *new PromotionMove(*this, idx, singleForwardMove, WKnight));
                addMove(white, *new PromotionMove(*this, idx, singleForwardMove, WBishop));
                addMove(white, *new PromotionMove(*this, idx, singleForwardMove, WRook));
                addMove(white, *new PromotionMove(*this, idx, singleForwardMove, WQueen));
            } else {
                addMove(white, *new SimpleMove(*this, idx, singleForwardMove));
            }
        }
        int file = Board::findFile(idx);
        for (int possibleMove : PAWN_MOVE_VECTORS) {
            int possiblePosition = idx + multiplier * possibleMove;
            if (!((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                  (file == 7 && (possibleMove == -7 || possibleMove == 9)))) {
                Square square = squareAt(possiblePosition);

                //normal capturing
                if (isOccupied(square)) {
                    if (white != isWhite(square)) {
                        if (rank == (white ? 1 : 6)) {
                            addMove(white, *new PromotionMove(*this, idx, possiblePosition, WKnight));
                            addMove(white, *new PromotionMove(*this, idx, possiblePosition, WBishop));
                            addMove(white, *new PromotionMove(*this, idx, possiblePosition, WRook));
                            addMove(white, *new PromotionMove(*this, idx, possiblePosition, WQueen));
                        } else {
                            addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                        }
                    }
                }

                //en passant
                else if (enPassantVirtual == possiblePosition){
                    int actualPawnPos = enPassantVirtual + (white ? 8 : -8);
                    if (isWhite(squareAt(actualPawnPos)) != white) {
                        addMove(white, *new EnPassantMove(*this, idx, enPassantVirtual));
                    }
                }
            }
        }
    }
}
void Board::findKnightMoves(int idx, bool white) {
    int file = findFile(idx);
    for (int possibleMove : KNIGHT_MOVE_VECTORS){
        if (((file == 0) && (possibleMove == -17 || possibleMove == -10 || possibleMove == 6 || possibleMove == 15)) ||
            ((file == 1) && (possibleMove == -10 || possibleMove == 6)) ||
            ((file == 6) && (possibleMove == -6  || possibleMove == 10)) ||
            ((file == 7) && (possibleMove == -15 || possibleMove == -6 || possibleMove == 10 || possibleMove == 17)))
            continue;
        int possiblePosition = idx + possibleMove;
        if (Board::isOnBoard(possiblePosition)){
            Square square = squareAt(possiblePosition);
            if (isOccupied(square)){
                if (isWhite(square) != white)
                    addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                continue;
            }
            else addMove(white, *new SimpleMove(*this, idx, possiblePosition));
        }
    }
}
void Board::findBishopMoves(int idx, bool white) {
    for (int possibleMove : BISHOP_MOVE_VECTORS) {
        int possiblePosition = idx;
        while (true) {
            int file = findFile(possiblePosition);
            if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                (file == 7 && (possibleMove == -7 || possibleMove == 9)))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = squareAt(possiblePosition);
            if (isOccupied(square)) {
                if (isWhite(square) != white)
                    addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                break;
            } else {
                addMove(white, *new SimpleMove(*this, idx, possiblePosition));
            }
        }
    }
}
void Board::findRookMoves(int idx, bool white) {
    for (int possibleMove : ROOK_MOVE_VECTORS) {
        int possiblePosition = idx;
        while (true) {
            int file = findFile(possiblePosition);
            if ((file == 0 && possibleMove == -1) ||
                (file == 7 && possibleMove == 1))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = squareAt(possiblePosition);
            if (isOccupied(square)) {
                if (isWhite(square) != white)
                    addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                break;
            } else {
                addMove(white, *new SimpleMove(*this, idx, possiblePosition));
            }
        }
    }
}
void Board::findQueenMoves(int idx, bool white) {
    for (int possibleMove : QUEEN_MOVE_VECTORS){
        int possiblePosition = idx;
        while(true){
            int file = findFile(possiblePosition);
            if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                (file == 7 && (possibleMove == -7 || possibleMove == 9)) ||
                (file == 0 && possibleMove == -1) ||
                (file == 7 && possibleMove == 1))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = squareAt(possiblePosition);
            if (isOccupied(square)) {
                if (isWhite(square) != white)
                    addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                break;
            } else {
                addMove(white, *new SimpleMove(*this, idx, possiblePosition));
            }
        }
    }
}
void Board::findKingMoves(int idx, bool white) {
    for (int rookFile : ROOK_FILES){
        if (!kingFirstMove(white) || isUnderAttack(idx, white))
            continue;
        int rookPosition = white ? rookFile + 56 : rookFile;
        Square rookSquare = squareAt(rookPosition);
        if (!isOccupied(rookSquare))
            continue;
        bool rookFirstMove = rookFile == 0 ? rookFile0FirstMove(white) : rookFile7FirstMove(white);
        if (rookSquare != getPiece(WRook, white) || !rookFirstMove)
            continue;
        if (isOccupiedOrUnderAttack(idx, rookPosition, white))
            continue;
        addMove(white, *new CastleMove(*this, idx, idx + (rookFile == 7 ? 2 : -2)));
    }

    for (int possibleMove : KING_MOVE_VECTORS){
        int file = findFile(idx);
        if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
            (file == 7 && (possibleMove == -7 || possibleMove == 9)) ||
            (file == 0 && possibleMove == -1) ||
            (file == 7 && possibleMove == 1))
            continue;
        int possiblePosition = idx + possibleMove;
        if (Board::isOnBoard(possiblePosition)) {
            Square square = squareAt(possiblePosition);
            if (isOccupied(square)) {
                if (isWhite(square) != white)
                    addMove(white, *new CaptureMove(*this, idx, possiblePosition));
                continue;
            } else {
                addMove(white, *new SimpleMove(*this, idx, possiblePosition));
            }
        }
    }
}

void Board::addMove(bool white, Move &move) {
    // perform checks:
    //   - doesn't result in check
    //   - king isn't already in check (block required)
    //   - checkmate ?
    if (white)
        whiteMoves.push_back(move);
    else blackMoves.push_back(move);
}

// TODO: Implement these functions
bool Board::kingFirstMove(bool white) {
    return true;
}
bool Board::rookFile0FirstMove(bool white) {
    return true;
}
bool Board::rookFile7FirstMove(bool white) {
    return true;
}

bool Board::isUnderAttack(int idx, bool white) {
    if (white)
        return blackDestinations[idx];
    return whiteDestinations[idx];
}
bool Board::isOccupiedOrUnderAttack(int startExc, int endExc, bool white) {
    if (startExc > endExc){
        int temp = startExc;
        startExc = endExc;
        endExc = temp;
    } startExc++; endExc--;
    for (int idx = startExc; idx <= endExc; idx++)
        if (isOccupied(idx) || isUnderAttack(idx, white))
            return true;
    return false;
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
Square Board::swapColour(Square piece) {
    return static_cast<Square>(-piece);
}
Square Board::getPiece(Square piece, bool white) {
    return static_cast<Square>(abs(piece) * (white ? 1 : -1));
}
