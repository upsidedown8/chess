package game.board.move.exceptions;

public class IllegalMoveException extends MoveException {
    public String toString(){
        return "Illegal move.";
    }
}