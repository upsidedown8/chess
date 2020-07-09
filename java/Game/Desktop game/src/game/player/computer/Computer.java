package game.player.computer;

import game.board.move.Move;
import game.player.Colour;
import game.player.Player;
import game.board.Board;

import java.util.List;

public class Computer extends Player {
    public static final String COMPUTER_NAME = "Computer";
    private Board hypotheticalBoard;

    Setting setting;
    Openings openings;
    PositionEvaluation positionEvaluation;

    public Computer(Colour computerColour, Setting setting){
        super(computerColour, COMPUTER_NAME);
        this.setting = setting;
        this.openings = new Openings();
        this.positionEvaluation = new PositionEvaluation();
        this.hypotheticalBoard = new Board();
    }
    public Move getNextMove(final Board board){
        final List<Move> possibleMoves = getPossibleMoves(board);
        Move bestMove = null;
        int bestScore = -100;
        for (Move move : possibleMoves){
            hypotheticalBoard = new Board(board);
            move.doMove(hypotheticalBoard);
            int score = positionEvaluation.scoreBoard(hypotheticalBoard, colour);
            if (score > bestScore){
                bestMove = move;
                bestScore = score;
            }
        }
        return bestMove;
    }

    public final List<Move> getPossibleMoves(Board board) {
        return colour.isWhite() ? board.whitePossibleMoves : board.blackPossibleMoves;
    }
    public final int[] getOppositeDestinations(Board board) {
        return colour.isWhite() ? board.blackDestinations : board.whiteDestinations;
    }
}
