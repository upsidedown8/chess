package game.board.move;

import game.board.Board;
import game.board.Square;
import game.board.piece.Piece;
import game.board.piece.PieceType;

public class Promotion extends Move{
    public PieceType promotionPieceType;

    PieceType endPieceType;
    boolean endPieceFirstMove;
    public Promotion(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
        final Square endSquare = board.getSquare(endPos);
        this.isCapture = endSquare.isOccupied();
        this.endPieceType = this.isCapture ? endSquare.piece.pieceType : null;
        if (isCapture)this.endPieceFirstMove = endSquare.piece.firstMove;
        else this.endPieceFirstMove = false;
        this.promotionPieceType = PieceType.QUEEN;
    }
    public Promotion(Promotion promotion){
        super(promotion);
        this.endPieceType = promotion.endPieceType;
        this.endPieceFirstMove = promotion.endPieceFirstMove;
        this.promotionPieceType = promotion.promotionPieceType;
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(promotionPieceType, endPos, startPieceColour);
        board.getSquare(endPos).piece.firstMove = true;
    }
    public void undoMove(Board board){
        if (isCapture){
            board.getSquare(endPos).piece = Piece.getPieceByPieceType(endPieceType, endPos, startPieceColour.reverse());
            board.getSquare(endPos).piece.firstMove = endPieceFirstMove;
        } else{
            board.getSquare(endPos).clear();
        }
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(startPieceType, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = startPieceFirstMove;
    }

    public Promotion copy(){
        return new Promotion(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder
                .append(MoveUtils.getAlgebraicPosition(startPos)).append(isCapture?'x':'-') //not necessarily
                .append(MoveUtils.getAlgebraicPosition(endPos));
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}