#pragma once
#include "game/board.hpp"
#include "game/position.hpp"
#include "game/move/move.hpp"
#include "game/move/moveStack.hpp"

enum GameStatus {
    WhiteToPlay,
    BlackToPlay,
    WhiteWon,
    BlackWon,
    Stalemate,
    Draw50Moves,
    ThreefoldRepetition
};

class Game {
public:
    GameStatus status;
    MoveStack *whiteMoves, *blackMoves;
    Position *moveCalculator;

    Game();
    void reset();

    MoveInfo makeMove(string& uci);
    MoveInfo makeMove(int start, int end);
    MoveInfo redoMove();
    MoveInfo undoMove();

    vector<int>& getMoveIndicators(int start);
    vector<int>& getPreviousHighlights();
};