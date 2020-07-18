#include "game/position.hpp"

Position::Position() {
    board = *new Board();
    reset();
}

void Position::reset() {
    enPassantVirtual = -1;
    board.reset();
    calcMoves();
}

void Position::doMove(Move *move) {
    move->doMove(board);
    calcMoves();
}
void Position::undoMove(Move *move) {
    move->undoMove(board);
    calcMoves();
}

vector<Move*> Position::getPossibleMoves(int idx) {
    vector<Move*> moves;
    Square square = board[idx];
    if (Board::isOccupied(square)) {
        vector<Move*> *playerMoves = Board::isWhite(square) ? &whiteMoves : &blackMoves;
        for (size_t i = 0; i < playerMoves->size(); i++) {
            Move *move = playerMoves->at(i);
            if (move->start == idx) {
                moves.push_back(move);
            }
        }
    }
    return moves;
}
void Position::findPawnMoves(int idx, bool white) {
    int rank = Board::findRank(idx);
    // move 1 square forward
    if (rank != 0 && rank != 7){
        int multiplier = white ? -1 : 1;
        int singleForwardMove = idx + multiplier * 8;
        if (!board.isOccupied(singleForwardMove)){
            // move 2 squares forward
            int doubleForwardMove = singleForwardMove + multiplier * 8;
            if (Board::isOnBoard(doubleForwardMove))
                if (!board.isOccupied(doubleForwardMove) && rank == (white ? 6 : 1))
                    addMove(white, new SimpleMove(board, idx, doubleForwardMove));
            if (rank == (white ? 1 : 6)) {
                addMove(white, new PromotionMove(board, idx, singleForwardMove, WKnight));
                addMove(white, new PromotionMove(board, idx, singleForwardMove, WBishop));
                addMove(white, new PromotionMove(board, idx, singleForwardMove, WRook));
                addMove(white, new PromotionMove(board, idx, singleForwardMove, WQueen));
            } else {
                addMove(white, new SimpleMove(board, idx, singleForwardMove));
            }
        }
        int file = Board::findFile(idx);
        for (int possibleMove : PAWN_MOVE_VECTORS) {
            int possiblePosition = idx + multiplier * possibleMove;
            if (!((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                  (file == 7 && (possibleMove == -7 || possibleMove == 9)))) {
                Square square = board[possiblePosition];

                //normal capturing
                if (Board::isOccupied(square)) {
                    if (white != Board::isWhite(square)) {
                        if (rank == (white ? 1 : 6)) {
                            addMove(white, new PromotionMove(board, idx, possiblePosition, WKnight));
                            addMove(white, new PromotionMove(board, idx, possiblePosition, WBishop));
                            addMove(white, new PromotionMove(board, idx, possiblePosition, WRook));
                            addMove(white, new PromotionMove(board, idx, possiblePosition, WQueen));
                        } else {
                            addMove(white, new CaptureMove(board, idx, possiblePosition));
                        }
                    }
                }

                //en passant
                else if (enPassantVirtual == possiblePosition){
                    int actualPawnPos = enPassantVirtual + (white ? 8 : -8);
                    if (Board::isWhite(board[actualPawnPos]) != white) {
                        addMove(white, new EnPassantMove(board, idx, enPassantVirtual));
                    }
                }
            }
        }
    }
}
void Position::findKnightMoves(int idx, bool white) {
    int file = Board::findFile(idx);
    for (int possibleMove : KNIGHT_MOVE_VECTORS){
        if (((file == 0) && (possibleMove == -17 || possibleMove == -10 || possibleMove == 6 || possibleMove == 15)) ||
            ((file == 1) && (possibleMove == -10 || possibleMove == 6)) ||
            ((file == 6) && (possibleMove == -6  || possibleMove == 10)) ||
            ((file == 7) && (possibleMove == -15 || possibleMove == -6 || possibleMove == 10 || possibleMove == 17)))
            continue;
        int possiblePosition = idx + possibleMove;
        if (Board::isOnBoard(possiblePosition)){
            Square square = board[possiblePosition];
            if (Board::isOccupied(square)){
                if (Board::isWhite(square) != white)
                    addMove(white, new CaptureMove(board, idx, possiblePosition));
                continue;
            }
            else addMove(white, new SimpleMove(board, idx, possiblePosition));
        }
    }
}
void Position::findBishopMoves(int idx, bool white) {
    for (int possibleMove : BISHOP_MOVE_VECTORS) {
        int possiblePosition = idx;
        while (true) {
            int file = Board::findFile(possiblePosition);
            if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                (file == 7 && (possibleMove == -7 || possibleMove == 9)))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = board[possiblePosition];
            if (Board::isOccupied(square)) {
                if (Board::isWhite(square) != white)
                    addMove(white, new CaptureMove(board, idx, possiblePosition));
                break;
            } else {
                addMove(white, new SimpleMove(board, idx, possiblePosition));
            }
        }
    }
}
void Position::findRookMoves(int idx, bool white) {
    for (int possibleMove : ROOK_MOVE_VECTORS) {
        int possiblePosition = idx;
        while (true) {
            int file = Board::findFile(possiblePosition);
            if ((file == 0 && possibleMove == -1) ||
                (file == 7 && possibleMove == 1))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = board[possiblePosition];
            if (Board::isOccupied(square)) {
                if (Board::isWhite(square) != white)
                    addMove(white, new CaptureMove(board, idx, possiblePosition));
                break;
            } else {
                addMove(white, new SimpleMove(board, idx, possiblePosition));
            }
        }
    }
}
void Position::findQueenMoves(int idx, bool white) {
    for (int possibleMove : QUEEN_MOVE_VECTORS){
        int possiblePosition = idx;
        while(true){
            int file = Board::findFile(possiblePosition);
            if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                (file == 7 && (possibleMove == -7 || possibleMove == 9)) ||
                (file == 0 && possibleMove == -1) ||
                (file == 7 && possibleMove == 1))
                break;
            possiblePosition += possibleMove;
            if (!Board::isOnBoard(possiblePosition))
                break;
            Square square = board[possiblePosition];
            if (Board::isOccupied(square)) {
                if (Board::isWhite(square) != white)
                    addMove(white, new CaptureMove(board, idx, possiblePosition));
                break;
            } else {
                addMove(white, new SimpleMove(board, idx, possiblePosition));
            }
        }
    }
}
void Position::findKingMoves(int idx, bool white) {
    for (int rookFile : ROOK_FILES){
        if (!kingFirstMove(white) || isUnderAttack(idx, white))
            continue;
        int rookPosition = white ? rookFile + 56 : rookFile;
        Square rookSquare = board[rookPosition];
        if (!Board::isOccupied(rookSquare))
            continue;
        bool rookFirstMove = rookFile == 0 ? rookFile0FirstMove(white) : rookFile7FirstMove(white);
        if (rookSquare != Board::getPiece(WRook, white) || !rookFirstMove)
            continue;
        if (isOccupiedOrUnderAttack(idx, rookPosition, white))
            continue;
        addMove(white, new CastleMove(board, idx, idx + (rookFile == 7 ? 2 : -2)));
    }

    for (int possibleMove : KING_MOVE_VECTORS){
        int file = Board::findFile(idx);
        if ((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
            (file == 7 && (possibleMove == -7 || possibleMove == 9)) ||
            (file == 0 && possibleMove == -1) ||
            (file == 7 && possibleMove == 1))
            continue;
        int possiblePosition = idx + possibleMove;
        if (Board::isOnBoard(possiblePosition)) {
            Square square = board[possiblePosition];
            if (Board::isOccupied(square)) {
                if (Board::isWhite(square) != white)
                    addMove(white, new CaptureMove(board, idx, possiblePosition));
                continue;
            } else {
                addMove(white, new SimpleMove(board, idx, possiblePosition));
            }
        }
    }
}

void Position::addMove(bool white, Move *move) {
    // perform checks:
    //   - doesn't result in check
    //   - king isn't already in check (block required)
    //   - checkmate ?
    if (white)
        whiteMoves.push_back(move);
    else blackMoves.push_back(move);
}

// TODO: Implement these functions
bool Position::kingFirstMove(bool white) {
    return true;
}
bool Position::rookFile0FirstMove(bool white) {
    return true;
}
bool Position::rookFile7FirstMove(bool white) {
    return true;
}

bool Position::isUnderAttack(int idx, bool white) {
    if (white)
        return blackDestinations[idx];
    return whiteDestinations[idx];
}
bool Position::isOccupiedOrUnderAttack(int startExc, int endExc, bool white) {
    if (startExc > endExc){
        int temp = startExc;
        startExc = endExc;
        endExc = temp;
    } startExc++; endExc--;
    for (int idx = startExc; idx <= endExc; idx++)
        if (board.isOccupied(idx) || isUnderAttack(idx, white))
            return true;
    return false;
}

void Position::calcMoves() {
    whiteMoves.clear();
    blackMoves.clear();
    int whiteKingPos = 0, blackKingPos = 0;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
        Square piece = board[i];
        if (Board::isOccupied(piece)) {
            bool white = Board::isWhite(piece);
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
        whiteDestinations[whiteMoves.at(i)->end] = true;
    for (size_t i = 0; i < blackMoves.size(); i++)
        blackDestinations[blackMoves.at(i)->end] = true;
    findKingMoves(whiteKingPos, true);
    findKingMoves(blackKingPos, false);
}