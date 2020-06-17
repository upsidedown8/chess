package game.board.move;

import game.board.Board;
import game.board.piece.Piece;

public class Simple extends Move{
    public Simple(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
    }
    public Simple(Simple simple){
        super(simple);
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(startPieceType, endPos, startPieceColour);
        board.getSquare(endPos).piece.firstMove = false;
    }
    public void undoMove(Board board){
        board.getSquare(endPos).clear();
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(startPieceType, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = startPieceFirstMove;
    }

    public Simple copy(){
        return new Simple(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        if (!startPieceType.isPawn())
            stringBuilder.append(startPieceType.getCode());
        stringBuilder
                .append(MoveUtils.getAlgebraicPosition(startPos)).append('-')
                .append(MoveUtils.getAlgebraicPosition(endPos));
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}