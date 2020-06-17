package game.board.move.exceptions;

public class TryToMoveSquareNotOnBoardException extends MoveException {
    public String toString(){
        return "The selected square is not on the board.";
    }
}
