package game.board.move.exceptions;

public class GameIsOverException extends MoveException {
    public String toString(){
        return "The game is over and therefore a move cannot be made.";
    }
}
