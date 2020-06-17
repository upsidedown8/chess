package com.example.chessclassic.game.board.move;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.Square;
import com.example.chessclassic.game.board.piece.Piece;
import com.example.chessclassic.game.board.piece.PieceType;

public class Capture extends Move{
    PieceType endPieceType;
    boolean endPieceFirstMove;
    public Capture(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
        Square endSquare = board.getSquare(endPos);
        this.endPieceType = endSquare.piece.pieceType;
        this.endPieceFirstMove = endSquare.piece.firstMove;
        this.isCapture = true;
    }
    public Capture(Capture capture){
        super(capture);
        this.endPieceType = capture.endPieceType;
        this.endPieceFirstMove = capture.endPieceFirstMove;
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(startPieceType, endPos, startPieceColour);
        board.getSquare(endPos).piece.firstMove = false;
    }
    public void undoMove(Board board){
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(startPieceType, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = startPieceFirstMove;
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(endPieceType, endPos, startPieceColour.reverse());
        board.getSquare(endPos).piece.firstMove = endPieceFirstMove;
    }

    public Capture copy(){
        return new Capture(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        if (!startPieceType.isPawn())
            stringBuilder.append(startPieceType.getCode());
        stringBuilder
                .append(MoveUtils.getAlgebraicPosition(startPos)).append('x')
                .append(MoveUtils.getAlgebraicPosition(endPos));
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}