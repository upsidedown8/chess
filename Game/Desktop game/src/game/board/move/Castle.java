package game.board.move;

import game.board.Board;
import game.board.piece.Piece;
import game.board.piece.PieceType;

public class Castle extends Move{
    boolean isKingSide;
    int rookStartPos, rookEndPos;
    public Castle(Board board, int startPos, int endPos){
        super(board, startPos, endPos);
        isKingSide = endPos - startPos == 2;
        rookStartPos = (isKingSide ? 7 : 0) + (startPieceColour.isWhite() ? 56 : 0);
        rookEndPos = isKingSide ? rookStartPos - 2 : rookStartPos + 3;
    }
    public Castle(Castle castle){
        super(castle);
        this.isKingSide = castle.isKingSide;
        this.rookStartPos = castle.rookStartPos;
        this.rookEndPos = castle.rookEndPos;
    }

    public void doMove(Board board){
        board.getSquare(startPos).clear();
        board.getSquare(endPos).piece = Piece.getPieceByPieceType(PieceType.KING, endPos, startPieceColour);
        board.getSquare(endPos).piece.firstMove = false;

        board.getSquare(rookStartPos).clear();
        board.getSquare(rookEndPos).piece = Piece.getPieceByPieceType(PieceType.ROOK, rookEndPos, startPieceColour);
        board.getSquare(rookEndPos).piece.firstMove = false;
    }
    public void undoMove(Board board){
        board.getSquare(endPos).clear();
        board.getSquare(startPos).piece = Piece.getPieceByPieceType(PieceType.KING, startPos, startPieceColour);
        board.getSquare(startPos).piece.firstMove = true;

        board.getSquare(rookEndPos).clear();
        board.getSquare(rookStartPos).piece = Piece.getPieceByPieceType(PieceType.ROOK, rookStartPos, startPieceColour);
        board.getSquare(rookStartPos).piece.firstMove = true;
    }

    public Castle copy(){
        return new Castle(this);
    }
    @Override
    public String toString(){
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(isKingSide ? "0-0" : "0-0-0");
        if (resultsMate)
            stringBuilder.append('#');
        else if (resultsCheck)
            stringBuilder.append('+');
        return stringBuilder.toString();
    }
}
