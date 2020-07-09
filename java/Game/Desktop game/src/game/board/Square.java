package game.board;

import game.board.piece.Piece;

public class Square {
    public Piece piece;

    public Square(){
        clear();
    }
    public Square(Piece piece){
        this.piece = piece.copy();
        this.piece.pos = piece.pos;
    }

    public void clear(){
        piece = null;
    }
    public boolean isOccupied(){
        return piece != null;
    }
    public Square copy(){
        return isOccupied() ? new Square(piece) : new Square();
    }
}
