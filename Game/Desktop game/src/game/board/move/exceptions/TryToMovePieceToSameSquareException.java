package game.board.move.exceptions;

public class TryToMovePieceToSameSquareException extends MoveException {
    public String toString(){
        return "Destination and starting squares are the same";
    }
}
