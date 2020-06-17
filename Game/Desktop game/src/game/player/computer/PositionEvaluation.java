package game.player.computer;

import game.board.Board;
import game.board.piece.PieceType;
import game.player.Colour;

public class PositionEvaluation {
    public PositionEvaluation(){

    }
    public int scoreBoard(Board board, Colour player){
        int whiteSum = 0;
        for (PieceType pieceType : board.whitePieces)
            whiteSum += pieceType.getValue();

        int blackSum = 0;
        for (PieceType pieceType : board.blackPieces)
            blackSum += pieceType.getValue();

        int score = whiteSum - blackSum;
        if (player.isBlack())
            score *= -1;
        return score;
    }
}
