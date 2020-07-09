package com.example.chessclassic.game.board.move;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.Square;
import com.example.chessclassic.game.board.piece.PieceType;
import com.example.chessclassic.game.player.Colour;

import java.util.List;

public abstract class Move {
    public final Colour startPieceColour;
    final PieceType startPieceType;
    public boolean isCapture, startPieceFirstMove, resultsCheck, resultsMate;
    public final int startPos, endPos;
    public int prevDraw50, prevEnPassantPawnVirtual;

    public Move(Board board, int startPos, int endPos){
        this.startPos = startPos;
        this.endPos = endPos;
        Square startSquare = board.getSquare(startPos);
        this.startPieceColour = startSquare.piece.colour;
        this.startPieceType = startSquare.piece.pieceType;
        this.startPieceFirstMove = startSquare.piece.firstMove;
        this.isCapture = false;
        this.prevEnPassantPawnVirtual = 0;
    }
    public Move(Move move){
        this.startPos = move.startPos;
        this.endPos = move.endPos;
        this.startPieceType = move.startPieceType;
        this.startPieceColour = move.startPieceColour;
        this.startPieceFirstMove = move.startPieceFirstMove;
        this.resultsCheck = move.resultsCheck;
        this.resultsMate = move.resultsMate;
        this.isCapture = move.isCapture;
    }

    public int getEnPassantPawn(Board board){
        Square endSquare = board.getSquare(this instanceof EnPassant ? ((EnPassant)this).virtualPawnPos : endPos);
        if (endSquare.piece.pieceType.isPawn() &&
            startPieceFirstMove &&
            Math.abs(endPos - startPos) == 16)
            return startPos + (startPieceColour.isWhite() ? -8 : 8);
        return -1;
    }
    public abstract Move copy();
    public boolean equals(Move move){
        return
                this.startPos == move.startPos &&
                this.endPos == move.endPos;
    }
    public abstract void doMove(Board board);
    public abstract void undoMove(Board board);

    public boolean isIn(List<Move> moves){
        for (Move move : moves) {
            if (move.equals(this)) {
                return true;
            }
        }
        return false;
    }
}