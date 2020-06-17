package game.player;

import game.board.Board;
import game.board.move.Move;

import java.util.List;

public class BlackPlayer extends Player {
    public BlackPlayer(){
        super(Colour.BLACK, "Black");
    }
    public BlackPlayer(String name){
        super(Colour.BLACK, name);
    }

    public final List<Move> getPossibleMoves(Board board) {
        return board.blackPossibleMoves;
    }
    public final int[] getOppositeDestinations(Board board) {
        return board.whiteDestinations;
    }
}