#pragma once
#include <vector>
#include <string>
#include "game/rle.hpp"
#include "game/move/move.hpp"
#include "game/move/moves/captureMove.hpp"
#include "game/move/moves/castleMove.hpp"
#include "game/move/moves/enPassantMove.hpp"
#include "game/move/moves/promotionMove.hpp"
#include "game/move/moves/simpleMove.hpp"

const char INITIAL_POSITION[] = "rnbqkbnr8p32#8PRNBQKBNR";

constexpr char   PAWN_NOTATION = 'p';
constexpr char KNIGHT_NOTATION = 'n';
constexpr char BISHOP_NOTATION = 'b';
constexpr char   ROOK_NOTATION = 'r';
constexpr char  QUEEN_NOTATION = 'q';
constexpr char   KING_NOTATION = 'k';
constexpr char  EMPTY_NOTATION = '#';

constexpr int  NUMBER_OF_SQUARES = 64;

constexpr int   PAWN_MOVE_VECTORS[] { 7, 9 };
constexpr int KNIGHT_MOVE_VECTORS[] { -17, -15, -10, -6, 6, 10, 15, 17 };
constexpr int BISHOP_MOVE_VECTORS[] { -9, -7, 7, 9 };
constexpr int   ROOK_MOVE_VECTORS[] { -8, -1, 1, 8 };
constexpr int  QUEEN_MOVE_VECTORS[] { -9, -8, -7, -1, 1, 7, 8, 9 };
constexpr int   KING_MOVE_VECTORS[] { -9, -8, -7, -1, 1, 7, 8, 9 };
constexpr int          ROOK_FILES[] { 0, 7 };

enum Square {
    Empty   =  0,
    BPawn   = -1, WPawn   = 1,
    BKnight = -2, WKnight = 2,
    BBishop = -3, WBishop = 3,
    BRook   = -4, WRook   = 4,
    BQueen  = -5, WQueen  = 5,
    BKing   = -6, WKing   = 6
};

using namespace std;

class Board {
private:
    Square squares[64];
    Square& squareAt(int idx);

    void findPawnMoves(int idx, bool white);
    void findKnightMoves(int idx, bool white);
    void findBishopMoves(int idx, bool white);
    void findRookMoves(int idx, bool white);
    void findQueenMoves(int idx, bool white);
    void findKingMoves(int idx, bool white);

    void addMove(bool white, Move &move);

    bool kingFirstMove(bool white);
    bool rookFile0FirstMove(bool white);
    bool rookFile7FirstMove(bool white);

    bool isUnderAttack(int squareCoord, bool white);
    bool isOccupiedOrUnderAttack(int startPosExc, int endPosExc, bool white);

public:
    int enPassantVirtual;
    bool whiteDestinations[64], blackDestinations[64];
    vector<Move> whiteMoves, blackMoves;

    Board();

    void reset();

    Square& operator[](int idx); 

    void calcMoves();
    void loadPosition(string board, bool encoded);
    void clearSquare(int idx);
    bool isOccupied(int idx);
    bool isOccupied(Square square);
    int getIdx(Square piece);
    
    string toString(bool encoded = true);

    vector<Move> getPossibleMoves(int idx);
    // bool isInCheck(Player* current);
    // bool isInCheckMate(Player* current);
    // bool isLowMaterial(Player* player);

    static int findRank(int pos);
    static int findFile(int pos);
    static bool isOnBoard(int pos);
    static bool isWhite(Square piece);
    static Square swapColour(Square piece);
    static Square getPiece(Square piece, bool white);
};