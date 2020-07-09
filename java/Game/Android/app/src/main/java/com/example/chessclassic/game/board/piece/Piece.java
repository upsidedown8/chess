package com.example.chessclassic.game.board.piece;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.player.Colour;

import java.util.List;

import static java.lang.Character.toUpperCase;


public abstract class Piece {
    public int pos;
    public boolean firstMove;
    public Colour colour;
    public PieceType pieceType;

    public Piece(int pos, Colour colour, boolean firstMove) {
        this.pos = pos;
        this.colour = colour;
        this.firstMove = firstMove;
    }

    public static PieceType getPieceTypeByNotation(char notation){
        for (PieceType pieceType : PieceType.values())
            if (pieceType.getCode() == toUpperCase(notation))
                return pieceType;
        return null;
    }
    public static Piece getPieceByPieceType(PieceType pieceType, int position, Colour colour){
        switch(pieceType){
            case PAWN: return new Pawn(position, colour);
            case BISHOP: return new Bishop(position, colour);
            case KNIGHT: return new Knight(position, colour);
            case ROOK: return new Rook(position, colour);
            case QUEEN: return new Queen(position, colour);
            case KING: return new King(position, colour);
            default: return null;
        }
    }

    public abstract Piece copy();
    public abstract List<Move> getPossibleMoves(Board board);

}
