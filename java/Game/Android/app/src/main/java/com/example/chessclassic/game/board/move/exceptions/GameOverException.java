package com.example.chessclassic.game.board.move.exceptions;

public class GameOverException extends MoveException {
    public String toString(){
        return "Cannot move, game is over.";
    }
}
