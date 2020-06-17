package game.board;

import game.board.move.Move;
import game.board.piece.*;
import game.player.*;

import java.util.*;

/**
 * stores the current positional data and contains functions
 * related to that data
 */
public class Board {
    public static final int NUMBER_OF_SQUARES = 64;
    static final String INITIAL_POSITION = "rnbqkbnr8p32#8PRNBQKBNR";

    public int enPassantPawnVirtual;
    public Colour enPassantColour;
    public List<Move> whitePossibleMoves, blackPossibleMoves;
    public List<PieceType> whitePieces, blackPieces;
    public int[] whiteDestinations, blackDestinations;
    public int whiteKingPos, blackKingPos;

    private Square[] board;

    public Board(){
        whitePieces = new ArrayList<PieceType>();
        blackPieces = new ArrayList<PieceType>();
        whitePossibleMoves = new ArrayList<Move>();
        blackPossibleMoves = new ArrayList<Move>();
        reset();
    }
    public Board(final Board board){
        this.board = new Square[NUMBER_OF_SQUARES];
        for (int i = 0; i < NUMBER_OF_SQUARES; i++)
            this.board[i] = board.getSquare(i).copy();
        whitePieces = new ArrayList<PieceType>();
        blackPieces = new ArrayList<PieceType>();
        whitePossibleMoves = new ArrayList<Move>();
        blackPossibleMoves = new ArrayList<Move>();
        this.enPassantPawnVirtual = board.enPassantPawnVirtual;
        this.enPassantColour = board.enPassantColour;
        setPossibleMoves();
    }
    public void reset(){
        enPassantPawnVirtual = -1;
        setPositions(INITIAL_POSITION, true);
        setPossibleMoves();
    }

    public void setPossibleMoves(){
        whitePossibleMoves.clear();      blackPossibleMoves.clear();
        whitePieces.clear();             blackPieces.clear();
        whiteDestinations = new int[NUMBER_OF_SQUARES]; blackDestinations = new int[NUMBER_OF_SQUARES];
        for (int i = 0; i < NUMBER_OF_SQUARES; i++){
            Square pieceSquare = getSquare(i);
            if (pieceSquare.isOccupied()){
                List<Move> piecePossibleMoves = pieceSquare.piece.getPossibleMoves(this);
                if (pieceSquare.piece.colour.isWhite()){
                    whitePieces.add(pieceSquare.piece.pieceType);
                    whitePossibleMoves.addAll(piecePossibleMoves);
                    for (Move move : piecePossibleMoves){
                        Square endSquare = getSquare(move.endPos);
                        whiteDestinations[move.endPos] = endSquare.isOccupied() && endSquare.piece.colour.isBlack() ? 2 : 1;
                    }
                }
                else{
                    blackPieces.add(pieceSquare.piece.pieceType);
                    blackPossibleMoves.addAll(piecePossibleMoves);
                    for (Move move : piecePossibleMoves){
                        Square endSquare = getSquare(move.endPos);
                        blackDestinations[move.endPos] = endSquare.isOccupied() && endSquare.piece.colour.isWhite() ? 2 : 1;
                    }
                }
            }
        }
    }
    public void setPositions(String board, boolean encoded){
        if (encoded)
            board = RLE.decode(board);
        this.board = new Square[NUMBER_OF_SQUARES];
        for (int i = 0; i < NUMBER_OF_SQUARES; i++){
            char notation = board.charAt(i);
            PieceType pieceType = Piece.getPieceTypeByNotation(notation);
            assert pieceType != null;
            if (!pieceType.isEmpty()){
                Colour colour = Character.isUpperCase(notation) ? Colour.WHITE : Colour.BLACK;
                this.board[i] = new Square(Objects.requireNonNull(Piece.getPieceByPieceType(pieceType, i, colour)));
            } else this.board[i] = new Square();
        }
    }
    public String toString(){
        return toString(true);
    }
    public String toString(boolean encoded){
        StringBuilder sb = new StringBuilder();
        for (Square square : board){
            if (square.isOccupied()) {
                char notation = square.piece.pieceType.getCode();
                sb.append(square.piece.colour.isWhite() ? notation : Character.toLowerCase(notation));
            } else { sb.append(PieceType.EMPTY.getCode()); }
        }
        String output = sb.toString();
        if (encoded)
            output = RLE.encode(output);
        return output;
    }
    public Square getSquare(int position){
        return board[position];
    }
    public void setEnPassantPawnVirtual(Move move){
        enPassantPawnVirtual = move.getEnPassantPawn(this);
        enPassantColour = move.startPieceColour;
    }

    public static boolean isOnBoard(int position){
        return  (position >= 0) && (position < NUMBER_OF_SQUARES);
    }
    public boolean isUnderAttack(int squareCoord, Colour enemyColour){
        return (enemyColour.isWhite() ? whiteDestinations : blackDestinations)[squareCoord] == 1;
    }
    public boolean isOccupiedOrUnderAttack(int startposExclusive, int finishposExclusive, Colour enemyColour){
        if (startposExclusive > finishposExclusive){
            int temp = startposExclusive;
            startposExclusive = finishposExclusive;
            finishposExclusive = temp;
        } startposExclusive++; finishposExclusive--;
        for (int squareCoord = startposExclusive; squareCoord <= finishposExclusive; squareCoord++){
            if (getSquare(squareCoord).isOccupied()) return true;
            if (isUnderAttack(squareCoord, enemyColour)) return true;
        }
        return false;
    }
    public boolean isInCheck(Player current){
        return current.getOppositeDestinations(this)[current.isWhite() ? whiteKingPos : blackKingPos] == 2;
    }
    public boolean isInCheckMate(Player current){
        return false;
    }
    public boolean isLowMaterial(Player player){
        List<PieceType> playerPieces = player.isWhite() ? whitePieces : blackPieces;
        int size = playerPieces.size();
        if (size <= 3) {
            // king only
            if (size == 0) return true;
            playerPieces.remove(PieceType.KING);
            if (size == 2){
                //king and knight or bishop
                if (playerPieces.get(0) == PieceType.KNIGHT ||
                    playerPieces.get(0) == PieceType.BISHOP)
                    return true;
            }
            //king and two knights
            return playerPieces.get(0) == PieceType.KNIGHT && playerPieces.get(1) == PieceType.KNIGHT;
        }
        return false;
    }
}