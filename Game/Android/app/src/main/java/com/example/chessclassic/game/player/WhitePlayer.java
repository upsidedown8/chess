package com.example.chessclassic.game.player;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.player.Player;

import java.util.List;

public class WhitePlayer extends Player {
    public WhitePlayer(){
        super(Colour.WHITE, "White");
    }
    public WhitePlayer(String name){
        super(Colour.WHITE, name);
    }

    public List<Move> getPossibleMoves(Board board) {
        return board.whitePossibleMoves;
    }
    public int[] getOppositeDestinations(Board board) {
        return board.blackDestinations;
    }
}