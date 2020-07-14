#pragma once
#include "board/rle.hpp"
#include <vector>
#include <string>

constexpr char PAWN_NOTATION   = 'p';
constexpr char KNIGHT_NOTATION = 'n';
constexpr char BISHOP_NOTATION = 'b';
constexpr char ROOK_NOTATION   = 'r';
constexpr char QUEEN_NOTATION  = 'q';
constexpr char KING_NOTATION   = 'k';

enum Square {
    Empty   = 0,
    BPawn   = -1, WPawn   = 1,
    BKnight = -2, WKnight = 2,
    BBishop = -3, WBishop = 3,
    BRook   = -4, WRook   = 4,
    BQueen  = -5, WQueen  = 5,
    BKing   = -6, WKing   = 6
};

class Board {
public:
    const char* INITIAL_POSITION  = "rnbqkbnr8p32#8PRNBQKBNR";
    Square squares[64];
    // int enPassantPawnVirtual;
    // PieceColor enPassantColour;
    // std::vector<Move> whitePossibleMoves, blackPossibleMoves;
    // std::vector<PieceType> whitePieces, blackPieces;
    // int whiteDestinations[64], blackDestinations[64];
    // int whiteKingPos, blackKingPos;

    // Board();
    // Board(Board* board);

    void reset();

    // void calcMoves();
    void loadPosition(std::string board, bool encoded);
    bool isOccupied(int idx);
    bool isWhite(Square piece);
    int getIdx(Square piece);
    // std::string toString(bool encoded = true);

    // bool isUnderAttack(int squareCoord, PieceColor enemyColor);
    // bool isOccupiedOrUnderAttack(int startPosExc, int endPosExc, PieceColor enemyColor);
    // bool isInCheck(Player* current);
    // bool isInCheckMate(Player* current);
    // bool isLowMaterial(Player* player);

    // static bool isOnBoard(int pos);
    // static int findRank(int pos);
    // static int findFile(int pos);
    // static int getPosFromAlgebraic(std::string algebraic);
    // static std::string getAlgebraicFromPos(int pos);
};