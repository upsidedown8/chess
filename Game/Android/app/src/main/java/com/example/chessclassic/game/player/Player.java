package com.example.chessclassic.game.player;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.board.move.exceptions.MoveException;
import com.example.chessclassic.game.board.move.exceptions.TryToRedoBeforeMoveMadeException;
import com.example.chessclassic.game.board.piece.PieceType;

import java.util.ArrayList;
import java.util.List;

public abstract class Player {
    public String name;
    public Colour colour;
    public List<Move> moves;
    public List<PieceType> captured;
    public int currentMove;
    public boolean hasResigned, hasTimedOut, inCheck, hasOfferedDraw, hasAcceptedDraw, hasAccepted50MoveDraw, hasAcceptedThreefoldDraw;

    public Player(Colour colour, String name){
        this.name = name;
        this.colour = colour;
        reset();
    }
    public void reset(){
        this.moves = new ArrayList<Move>();
        this.captured = new ArrayList<PieceType>();
        this.currentMove = -1;
        this.hasResigned = false;
        this.hasTimedOut = false;
        this.hasOfferedDraw = false;
        this.hasAcceptedDraw = false;
        this.hasAccepted50MoveDraw = false;
        this.hasAcceptedThreefoldDraw = false;
        this.inCheck = false;
    }
    public boolean isWhite(){
        return colour.isWhite();
    }
    public boolean isBlack(){
        return colour.isBlack();
    }

    public abstract List<Move> getPossibleMoves(Board board);
    public abstract int[] getOppositeDestinations(Board board);

    public Move previousMove() throws MoveException{
        if (!moves.isEmpty() && currentMove > 0){
            currentMove--;
            return moves.get(currentMove);
        } else throw new TryToRedoBeforeMoveMadeException();
    }
    public Move nextMove() throws MoveException {
        if (!moves.isEmpty() && currentMove < moves.size()){
            currentMove++;
            return moves.get(currentMove);
        } else throw new TryToRedoBeforeMoveMadeException();
    }
    public void addMove(Move move){
        moves.add(move);
        currentMove++;
    }
    public void addEnemyPiece(PieceType piece){
        captured.add(piece);
    }
    public void removeEnemyPiece(PieceType piece){
        captured.remove(piece);
    }
}