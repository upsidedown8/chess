package com.example.chessclassic.game.board.move.exceptions;

public class TryToUndoBeforeMoveMadeException extends MoveException {
    public String toString(){
        return "Undo limit reached.";
    }
}
