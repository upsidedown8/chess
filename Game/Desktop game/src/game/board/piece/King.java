package game.board.piece;

import game.board.Board;
import game.board.Square;
import game.board.move.*;
import game.player.Colour;

import java.util.ArrayList;
import java.util.List;

public class King extends Piece {
    final int[] POSSIBLE_MOVES = { -9, -8, -7, -1, 1, 7, 8, 9 };
    final int[] ROOK_FILES = { 0, 7 };
    public King(int position, Colour colour) {
        super(position, colour, true);
        this.pieceType = PieceType.KING;
    }
    public King(int position, Colour colour, boolean firstMove) {
        super(position, colour, firstMove);
        this.pieceType = PieceType.KING;
    }
    public King copy(){
        return new King(pos, colour, firstMove);
    }

    @Override
    public List<Move> getPossibleMoves(Board board) {
        List<Move> possibleMoves = new ArrayList<Move>();
        for (int rookFile : ROOK_FILES){
            if (!this.firstMove) continue;
            int rookPosition = this.colour.isWhite() ? rookFile + 56 : rookFile;
            final Square rookStartingSquare = board.getSquare(rookPosition);
            if (!rookStartingSquare.isOccupied()) continue;
            if (!(rookStartingSquare.piece.pieceType.isRook() && rookStartingSquare.piece.firstMove)) continue;
            if (board.isUnderAttack(this.pos, colour.reverse())) continue;
            if (board.isOccupiedOrUnderAttack(this.pos, rookPosition, colour.reverse())) continue;
            possibleMoves.add(new Castle(board, this.pos, rookFile == 7 ? this.pos + 2 : this.pos - 2));
        }
        for (int possibleMove : POSSIBLE_MOVES){
            int file = MoveUtils.findFile(this.pos);
            if (  (file == 0 && (possibleMove == -9 || possibleMove == 7)) ||
                  (file == 7 && (possibleMove == -7 || possibleMove == 9)) ||
                  (file == 0 && possibleMove == -1) ||
                  (file == 7 && possibleMove == 1))
                continue;
            int possiblePosition = this.pos + possibleMove;
            if (Board.isOnBoard(possiblePosition)) {
                final Square square = board.getSquare(possiblePosition);
                if (square.isOccupied()) {
                    if (square.piece.colour != this.colour)
                        possibleMoves.add(new Capture(board, this.pos, possiblePosition));
                    continue;
                }
                else {
                    possibleMoves.add(new Simple(board, this.pos, possiblePosition));
                }
            }
        }
        return possibleMoves;
    }
}
