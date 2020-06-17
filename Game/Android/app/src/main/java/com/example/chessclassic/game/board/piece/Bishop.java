package com.example.chessclassic.game.board.piece;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.Square;
import com.example.chessclassic.game.board.move.Capture;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.board.move.MoveUtils;
import com.example.chessclassic.game.board.move.Simple;
import com.example.chessclassic.game.player.Colour;

import java.util.ArrayList;
import java.util.List;

public class Bishop extends Piece {
    final int[] POSSIBLE_MOVES = { -9, -7, 7, 9 };
    public Bishop(int position, Colour colour) {
        super(position, colour, true);
        pieceType = PieceType.BISHOP;
    }
    public Bishop(int position, Colour colour, boolean firstMove){
        super(position, colour, firstMove);
        pieceType = PieceType.BISHOP;
    }
    public Bishop copy(){
        return new Bishop(this.pos, this.colour, this.firstMove);
    }

    @Override
    public List<Move> getPossibleMoves(Board board) {
        List<Move> possibleMoves = new ArrayList<Move>();
        for (int possibleMove : POSSIBLE_MOVES){
            int possiblePosition = this.pos;
            while(true){
                int file = MoveUtils.findFile(possiblePosition);
                if ( (file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                     (file == 7 && (possibleMove == -7 || possibleMove == 9)))
                    break;
                possiblePosition += possibleMove;
                if (!Board.isOnBoard(possiblePosition)) break;
                Square square = board.getSquare(possiblePosition);
                if (square.isOccupied()) {
                    if (square.piece.colour != this.colour)
                        possibleMoves.add(new Capture(board, this.pos, possiblePosition));
                    break;
                }
                else possibleMoves.add(new Simple(board, this.pos, possiblePosition));
            }
        }
        return possibleMoves;
    }
}
