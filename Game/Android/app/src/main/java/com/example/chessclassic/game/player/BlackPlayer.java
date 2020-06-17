package com.example.chessclassic.game.player;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.move.Move;

import java.util.List;

public class BlackPlayer extends Player {
    public BlackPlayer(){
        super(Colour.BLACK, "Black");
    }
    public BlackPlayer(String name){
        super(Colour.BLACK, name);
    }

    public List<Move> getPossibleMoves(Board board) {
        return board.blackPossibleMoves;
    }
    public int[] getOppositeDestinations(Board board) {
        return board.whiteDestinations;
    }
}