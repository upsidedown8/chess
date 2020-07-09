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

public class Knight extends Piece {
    final int[] POSSIBLE_MOVES = { -17, -15, -10, -6, 6, 10, 15, 17 };
    public Knight(int position, Colour colour) {
        super(position, colour, true);
        pieceType = PieceType.KNIGHT;
    }
    public Knight(int position, Colour colour, boolean firstMove) {
        super(position, colour, firstMove);
        pieceType = PieceType.KNIGHT;
    }
    public Knight copy(){
        return new Knight(this.pos, this.colour, this.firstMove);
    }

    @Override
    public List<Move> getPossibleMoves(Board board) {
        List<Move> possibleMoves = new ArrayList<Move>();
        int file = MoveUtils.findFile(this.pos);
        for (int possibleMove : POSSIBLE_MOVES){
            if (((file == 0) && (possibleMove == -17 || possibleMove == -10 || possibleMove == 6 || possibleMove == 15)) ||
                ((file == 1) && (possibleMove == -10 || possibleMove == 6)) ||
                ((file == 6) && (possibleMove == -6 || possibleMove == 10)) ||
                ((file == 7) && (possibleMove == -15 || possibleMove == -6 || possibleMove == 10 || possibleMove == 17)))
                continue;
            int possiblePosition = this.pos + possibleMove;
            if (Board.isOnBoard(possiblePosition)){
                Square square = board.getSquare(possiblePosition);
                if (square.isOccupied() && square.piece.colour != this.colour)
                    possibleMoves.add(new Capture(board, this.pos, possiblePosition));
                else possibleMoves.add(new Simple(board, this.pos, possiblePosition));
            }
        }
        return possibleMoves;
    }
}
