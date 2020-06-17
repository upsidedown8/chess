package com.example.chessclassic.game.board.move;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.piece.Piece;
import com.example.chessclassic.game.board.piece.PieceType;

public class EnPassant extends Move{
    int virtualPawnPos;
    public EnPassant(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
        this.virtualPawnPos = endPos + (startPieceColour.reverse().isWhite() ? 8 : -8);
        this.isCapture = true;
    }
    public EnPassant(EnPassant enPassant){
        super(enPassant);
        this.virtualPawnPos = enPassant.virtualPawnPos;
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).clear();
        board.getSquare(virtualPawnPos).piece = Piece.getPieceByPieceType(startPieceType, virtualPawnPos, startPieceColour);
        board.getSquare(virtualPawnPos).piece.firstMove = false;
    }
    public void undoMove(Board board){
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(startPieceType, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = startPieceFirstMove;
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(PieceType.PAWN, endPos, startPieceColour.reverse());
        board.getSquare(endPos).piece.firstMove = false;
        board.getSquare(virtualPawnPos).clear();
    }

    public EnPassant copy(){
        return new EnPassant(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder
                .append(MoveUtils.getAlgebraicPosition(startPos)).append('x')
                .append(MoveUtils.getAlgebraicPosition(virtualPawnPos));
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}
