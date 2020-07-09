package game.board.piece;

import game.board.Board;
import game.board.Square;
import game.board.move.*;
import game.player.Colour;

import java.util.ArrayList;
import java.util.List;

public class Pawn extends Piece {
    final int[] POSSIBLE_MOVES = { 7, 9 };
    public Pawn(int position, Colour colour) {
        super(position, colour, true);
        pieceType = PieceType.PAWN;
    }
    public Pawn(int position, Colour colour, boolean firstMove) {
        super(position, colour, firstMove);
        pieceType = PieceType.PAWN;
    }
    public Pawn copy(){
        return new Pawn(this.pos, this.colour, this.firstMove);
    }

    @Override
    public List<Move> getPossibleMoves(Board board) {
        List<Move> possibleMoves = new ArrayList<Move>();
        int rank = MoveUtils.findRank(this.pos);
        // move 1 square forward
        if (!(rank == 0 || rank == 7)){
            int multiplier = colour.isWhite() ? -1 : 1;
            if (!board.getSquare(pos + multiplier * 8).isOccupied()){
                // move 2 squares forward
                int newPosition = pos + multiplier * 16;
                if (Board.isOnBoard(newPosition)){
                    if (!board.getSquare(newPosition).isOccupied() && firstMove){
                        possibleMoves.add(new Simple(board, pos, pos + multiplier * 16));
                    }
                }
                if (rank == (this.colour.isWhite() ? 1 : 6))
                    possibleMoves.add(new Promotion(board, pos, pos + multiplier * 8));
                else
                    possibleMoves.add(new Simple(board, pos, pos + multiplier * 8));
            }
            int file = MoveUtils.findFile(pos);
            for (int possibleMove : POSSIBLE_MOVES){
                int possiblePosition = pos + multiplier * possibleMove;
                if (!((file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                        (file == 7 && (possibleMove == -7 || possibleMove == 9)))) {
                    final Square square = board.getSquare(possiblePosition);
                    //normal capturing
                    if (square.isOccupied()) {
                        if (square.piece.colour != colour) {
                            if (rank == (this.colour.isWhite() ? 1 : 6))
                                possibleMoves.add(new Promotion(board, pos, possiblePosition));
                            else
                                possibleMoves.add(new Capture(board, pos, possiblePosition));
                        }
                    }

                    //en passant
                    else if (board.enPassantPawnVirtual == possiblePosition){
                        int actualPawnPos = board.enPassantPawnVirtual + (board.enPassantColour.isWhite() ? -8 : 8);
                        if (board.getSquare(actualPawnPos).piece.colour != colour) {
                            possibleMoves.add(new EnPassant(board, pos, actualPawnPos));
                        }
                    }
                }
            }
        }
        return possibleMoves;
    }
}
