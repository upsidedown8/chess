#include "game/game.hpp"

Game::Game() {
    board = *new Board();
    reset();
}
void Game::reset() {
    board.reset();
}

MoveInfo Game::makeMove(string uci) {

}
MoveInfo Game::makeMove(int start, int end) {
    
}
MoveInfo Game::redoMove() {
    
}
MoveInfo Game::undoMove() {

}

vector<int> Game::getMoveIndicators(int start) {
    return *new vector<int>;
}
vector<int> Game::getPreviousHighlights() {
    return *new vector<int>;
}
