#include "game/game.hpp"

Game::Game() {
    moveCalculator = *new Position();
    reset();
}
void Game::reset() {
    moveCalculator.reset();
}

MoveInfo Game::makeMove(string uci) {

}
MoveInfo Game::makeMove(int start, int end) {
    Move *theMove;
    auto possibleMoves = moveCalculator.getPossibleMoves(start);
    for (auto &move : possibleMoves)
        if (move.get()->start == start && move.get()->end == end)
            theMove = move.get();
    moveCalculator.doMove(*theMove);
}
MoveInfo Game::redoMove() {
    
}
MoveInfo Game::undoMove() {

}

vector<int> Game::getMoveIndicators(int start) {
    vector<int> moveIndicators;
    auto possibleMoves = moveCalculator.getPossibleMoves(start);
    for (auto &move : possibleMoves)
        moveIndicators.push_back(move.get()->end);
    return moveIndicators;
}
vector<int> Game::getPreviousHighlights() {
    return *new vector<int>;
}
