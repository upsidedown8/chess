package com.example.chessclassic.game.board.move.exceptions;

public class EmptySquareSelectedException extends MoveException {
    public String toString(){
        return "The selected square must contain a piece.";
    }
}
