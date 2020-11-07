#pragma once
#include <vector>
#include "game/bitboard.hpp"

enum Piece {
    WPawn
};

typedef struct {
    uchar start, end;
} Move;

typedef struct {
    Move move;
    uchar castleInfo;
    uchar enPassant;
    int fiftyMove;
} CompleteMove;

class Board {
private:
    BitBoard bitboard;
    CompleteMove currentInfo;
    std::vector<CompleteMove> history;

    void updateHelpers();

public:
    U64 W_PIECES, B_PIECES, PIECES;

    Board();
    
    void reset();

    std::string toString();

    CompleteMove getMoveInfo(Move &move);

    bool   doMove(const CompleteMove &moveInfo);
    bool undoMove(const CompleteMove &moveInfo);

};