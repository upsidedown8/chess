package game.board.move.exceptions;

public class TryToTakeFriendlyPieceException extends MoveException {
    public String toString(){
        return "A player cannot capture their own piece.";
    }
}
