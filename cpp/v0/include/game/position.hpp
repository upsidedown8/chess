#pragma once
#include <memory>
#include "game/board.hpp"
#include "game/move/move.hpp"
#include "game/move/moves/captureMove.hpp"
#include "game/move/moves/castleMove.hpp"
#include "game/move/moves/enPassantMove.hpp"
#include "game/move/moves/promotionMove.hpp"
#include "game/move/moves/simpleMove.hpp"

class Position {
private:
    void findPawnMoves(int idx, bool white);
    void findKnightMoves(int idx, bool white);
    void findBishopMoves(int idx, bool white);
    void findRookMoves(int idx, bool white);
    void findQueenMoves(int idx, bool white);
    void findKingMoves(int idx, bool white);

    void addMove(bool white, Move *move);

    bool kingFirstMove(bool white);
    bool rookFile0FirstMove(bool white);
    bool rookFile7FirstMove(bool white);

    bool isUnderAttack(int squareCoord, bool white);
    bool isOccupiedOrUnderAttack(int startPosExc, int endPosExc, bool white);

public:
    int enPassantVirtual;
    bool whiteDestinations[64], blackDestinations[64];
    vector<Move*> whiteMoves, blackMoves;
    Board board;

    Position();

    void reset();

    void calcMoves();
    vector<Move*> getPossibleMoves(int idx);

    void doMove(Move *move);
    void undoMove(Move *move);

    // bool isInCheck(Player* current);
    // bool isInCheckMate(Player* current);
    // bool isLowMaterial(Player* player);
};