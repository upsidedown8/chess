#include "game/board.hpp"
#include "game/move/move.hpp"

class Game {
public:
    Board board;
    
    Game();
    void reset();

    MoveInfo makeMove(string uci);
    MoveInfo makeMove(int start, int end);
    MoveInfo redoMove();
    MoveInfo undoMove();

    vector<int> getMoveIndicators(int start);
    vector<int> getPreviousHighlights();
};