package com.example.chessclassic.game.player.computer;

import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.player.Colour;
import com.example.chessclassic.game.player.Player;
import com.example.chessclassic.game.board.Board;

import java.util.List;

public class Computer extends Player {
    public static final String COMPUTER_NAME = "Computer";

    Setting setting;
    Openings openings;
    PositionEvaluation positionEvaluation;

    public Computer(Colour computerColour, Setting setting){
        super(computerColour, COMPUTER_NAME);
        this.setting = setting;
        this.openings = new Openings();
        this.positionEvaluation = new PositionEvaluation();
    }

    public List<Move> getPossibleMoves(Board board) {
        return colour.isWhite() ? board.whitePossibleMoves : board.blackPossibleMoves;
    }
    public int[] getOppositeDestinations(Board board) {
        return colour.isWhite() ? board.blackDestinations : board.whiteDestinations;
    }
}
