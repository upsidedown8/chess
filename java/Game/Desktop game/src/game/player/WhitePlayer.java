package game.player;

import game.board.Board;
import game.board.move.Move;
import game.player.Player;

import java.util.List;

public class WhitePlayer extends Player {
    public WhitePlayer(){
        super(Colour.WHITE, "White");
    }
    public WhitePlayer(String name){
        super(Colour.WHITE, name);
    }

    public final List<Move> getPossibleMoves(Board board) {
        return board.whitePossibleMoves;
    }
    public final int[] getOppositeDestinations(Board board) {
        return board.blackDestinations;
    }
}