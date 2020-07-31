#include "game/game.hpp"

Game::Game() {
    moveCalculator = new Position();
    reset();
}
void Game::reset() {
    moveCalculator->reset();
    status = WhiteToPlay;
}

MoveInfo Game::makeMove(string& uci) {

}
MoveInfo Game::makeMove(int start, int end) {
    Move *theMove;
    auto possibleMoves = moveCalculator->getPossibleMoves(start);
    for (Move *move : possibleMoves)
        if (move->start == start && move->end == end)
            theMove = move;
    moveCalculator->doMove(theMove);
}
MoveInfo Game::redoMove() {
    
}
MoveInfo Game::undoMove() {

}

vector<int>& Game::getMoveIndicators(int start) {
    vector<int> *moveIndicators = new vector<int>;
    auto possibleMoves = moveCalculator->getPossibleMoves(start);
    for (Move *move : possibleMoves)
        moveIndicators->push_back(move->end);
    return *moveIndicators;
}
vector<int>& Game::getPreviousHighlights() {
    return *new vector<int>;
}
