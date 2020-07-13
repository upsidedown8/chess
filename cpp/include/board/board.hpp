#pragma once
#include "board/square.hpp"
#include "board/piece/piece.hpp"
#include "board/rle.hpp"
#include <vector>
#include <string>

struct Square {
    Piece* piece;
    
};

class Board {
public:
    const char* INITIAL_POSITION  = "rnbqkbnr8p32#8PRNBQKBNR";

    Square board[64];
    int enPassantPawnVirtual;
    PieceColor enPassantColour;
    std::vector<Move> whitePossibleMoves, blackPossibleMoves;
    std::vector<PieceType> whitePieces, blackPieces;
    int whiteDestinations[64], blackDestinations[64];
    int whiteKingPos, blackKingPos;

    Board();
    Board(Board* board);

    void reset();

    void calcMoves();
    void loadPosition(std::string board, bool encoded);
    std::string toString(bool encoded = true);

    bool isUnderAttack(int squareCoord, PieceColor enemyColor);
    bool isOccupiedOrUnderAttack(int startPosExc, int endPosExc, PieceColor enemyColor);
    bool isInCheck(Player* current);
    bool isInCheckMate(Player* current);
    bool isLowMaterial(Player* player);

    static bool isOnBoard(int pos);
    static int findRank(int pos);
    static int findFile(int pos);
    static int getPosFromAlgebraic(std::string algebraic);
    static std::string getAlgebraicFromPos(int pos);
};