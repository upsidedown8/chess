#pragma once
#include "game/board/square.hpp"
#include "game/board/piece/piece.hpp"
#include <vector>
#include <string>

class Board {
private:
    Square board[64];

public:
    const int NUMBER_OF_SQUARES = 64;
    const char* INITIAL_POSITION = "rnbqkbnr8p32#8PRNBQKBNR";

    int enPassantPawnVirtual;
    PieceColor enPassantColour;
    std::vector<Move> whitePossibleMoves, blackPossibleMoves;
    std::vector<PieceType> whitePieces, blackPieces;
    int* whiteDestinations, blackDestinations;
    int whiteKingPos, blackKingPos;

    Board();
    Board(Board* board);

    void reset();

    void setPossibleMoves();
    void setPositions(std::string board, bool encoded);
    std::string toString();
    std::string toString(bool encoded);
    Square* getSquare(int pos);

    bool isUnderAttack(int squareCoord, PieceColor enemyColor);
    bool isOccupiedOrUnderAttack(int startPosExc, int endPosExc, PieceColor enemyColor);
    bool isInCheck(Player* current);
    bool isInCheckMate(Player* current);
    bool isLowMaterial(Player* player);

    static bool isOnBoard(int pos);
    static int findRank(int pos);
    static int findFile(int pos);
    static int getAlgebraicFromPos(int pos);
    static int getPosFromAlgebraic(std::string algebraic);
};