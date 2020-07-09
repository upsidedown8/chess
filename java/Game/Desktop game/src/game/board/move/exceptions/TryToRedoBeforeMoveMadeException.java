package game.board.move.exceptions;

public class TryToRedoBeforeMoveMadeException extends MoveException {
    public String toString(){
        return "Redo limit reached.";
    }
}
