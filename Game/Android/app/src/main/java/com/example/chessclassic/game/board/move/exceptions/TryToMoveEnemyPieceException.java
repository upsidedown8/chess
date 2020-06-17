package com.example.chessclassic.game.board.move.exceptions;

public class TryToMoveEnemyPieceException extends MoveException {
    public String toString(){
        return "Players can only move pieces of their own colour .";
    }
}
