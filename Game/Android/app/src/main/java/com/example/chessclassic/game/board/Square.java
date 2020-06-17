package com.example.chessclassic.game.board;

import com.example.chessclassic.game.board.piece.Piece;

public class Square {
    public int pos;
    public Piece piece;
    public Square(int pos){
        this.pos = pos;
        clear();
    }
    public Square(Piece piece, int pos){
        this.piece = piece.copy();
        this.piece.pos = pos;
        this.pos = pos;
    }

    public void clear(){
        piece = null;
    }
    public boolean isOccupied(){
        return piece != null;
    }
    public Square copy(){
        return new Square(piece, pos);
    }
}
