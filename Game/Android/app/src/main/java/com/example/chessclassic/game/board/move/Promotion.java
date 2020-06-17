package com.example.chessclassic.game.board.move;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.Square;
import com.example.chessclassic.game.board.piece.Piece;
import com.example.chessclassic.game.board.piece.PieceType;

public class Promotion extends Move{
    public PieceType promotionPieceType;

    PieceType endPieceType;
    boolean endPieceFirstMove;
    public Promotion(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
        Square endSquare = board.getSquare(endPos);
        this.isCapture = endSquare.isOccupied();
        this.endPieceType = this.isCapture ? endSquare.piece.pieceType : null;
        this.endPieceFirstMove = endSquare.piece.firstMove;
        this.promotionPieceType = PieceType.QUEEN;
    }
    public Promotion(Promotion promotion){
        super(promotion);
        this.endPieceType = promotion.endPieceType;
        this.endPieceFirstMove = promotion.endPieceFirstMove;
        this.promotionPieceType = promotion.promotionPieceType;
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(promotionPieceType, endPos, startPieceColour);
        board.getSquare(endPos).piece.firstMove = true;
    }
    public void undoMove(Board board){
        if (isCapture){
            board.getSquare(endPos).piece = Piece.getPieceByPieceType(endPieceType, endPos, startPieceColour.reverse());
            board.getSquare(endPos).piece.firstMove = endPieceFirstMove;
        } else{
            board.getSquare(endPos).clear();
        }
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(startPieceType, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = startPieceFirstMove;
    }

    public Promotion copy(){
        return new Promotion(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder
                .append(MoveUtils.getAlgebraicPosition(startPos)).append(isCapture?'x':'-') //not necessarily
                .append(MoveUtils.getAlgebraicPosition(endPos));
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}