package com.example.chessclassic.game;

import com.example.chessclassic.game.board.Board;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.board.move.exceptions.*;
import com.example.chessclassic.game.player.*;

import java.util.ArrayList;
import java.util.List;

public class Chess {
    public Board board;
    public Player currentPlayer;
    public WhitePlayer white;
    public BlackPlayer black;
    public GameStatus status;
    public boolean gameIsOver, draw50Open, drawThreefoldOpen;
    int draw50count;
    List<String> positions;

    public Chess(WhitePlayer white, BlackPlayer black){
        this.white = white;
        this.black = black;
        this.board = new Board();
        reset();
    }
    public void reset(){
        board.reset();
        white.reset();
        black.reset();
        draw50count = 0;
        currentPlayer = white;
        status = GameStatus.WHITE_TO_PLAY;
        positions = new ArrayList<String>();
        positions.add(board.toString());
        gameIsOver = false;
        draw50Open = false;
        drawThreefoldOpen = false;
    }

    public void move(Move move) throws MoveException{
        if (!gameIsOver){
            if (move.isCapture)
                currentPlayer.addEnemyPiece(board.getSquare(move.endPos).piece.pieceType);
            currentPlayer.addMove(move);
            updatePlayer();
            updateDraw50Count(move);
            move.doMove(board);
            board.setEnPassantPawnVirtual(move);
            positions.add(board.toString());
            updateGameStatus();
        } else throw new GameIsOverException();
    }
    public void undo() throws MoveException{
        if (!gameIsOver){
            updatePlayer();
            Move previousMove;
            try{
                previousMove = currentPlayer.previousMove();
                previousMove.undoMove(board);
                if (previousMove.isCapture)
                    currentPlayer.removeEnemyPiece(board.getSquare(previousMove.endPos).piece.pieceType);
                draw50count = previousMove.prevDraw50;
                board.enPassantPawnVirtual = previousMove.prevEnPassantPawnVirtual;
                positions.remove(positions.size()-1);
                updateGameStatus();
            } catch (MoveException moveException){
                updatePlayer();
                throw moveException;
            }
        } else throw new GameIsOverException();
    }
    public void redo() throws MoveException{
        if (!gameIsOver) {
            Move nextMove;
            nextMove = currentPlayer.nextMove();
            if (nextMove.isCapture)
                currentPlayer.addEnemyPiece(board.getSquare(nextMove.endPos).piece.pieceType);
            currentPlayer.addMove(nextMove);
            updatePlayer();
            updateDraw50Count(nextMove);
            nextMove.doMove(board);
            board.setEnPassantPawnVirtual(nextMove);
            positions.add(board.toString());
            updateGameStatus();
        } else throw new GameIsOverException();
    }
    public Move getMove(int start, int end) throws MoveException {
        if (!Board.isOnBoard(start) || !Board.isOnBoard(end)) throw new TryToMoveSquareNotOnBoardException();
        if (!board.getSquare(start).isOccupied()) throw new EmptySquareSelectedException();
        if (board.getSquare(start).piece.colour != currentPlayer.colour) throw new TryToMoveEnemyPieceException();
        Move yourMove = null;
        for (Move move : currentPlayer.getPossibleMoves(board)) {
            if (move.startPos == start){
                if (move.endPos == end || (end == board.enPassantPawnVirtual && board.getSquare(start).piece.pieceType.isPawn() && move.isCapture)) {
                    yourMove = move;
                    break;
                }
            }
        }
        if (yourMove == null) throw new IllegalMoveException();
        yourMove.prevDraw50 = draw50count;
        yourMove.prevEnPassantPawnVirtual = board.enPassantPawnVirtual;
        return yourMove;
    }

    public void timeOut(Player player){
        if (!gameIsOver){
            player.hasTimedOut = true;
            updateGameStatus();
        }
    }
    public void resign(Player player){
        if (!gameIsOver) {
            player.hasResigned = true;
            updateGameStatus();
        }
    }
    public void offerDraw(Player player){
        if (!gameIsOver && currentPlayer == player) {
            player.hasOfferedDraw = true;
        }
    }
    public void acceptDraw(Player player){
        if (!gameIsOver && getOtherPlayer(player).hasOfferedDraw) {
            player.hasAcceptedDraw = true;
            updateGameStatus();
        }
    }
    public void acceptThreefoldDraw(Player player){
        if (drawThreefoldOpen){
            player.hasAcceptedThreefoldDraw = true;
        }
    }
    public void acceptDraw50(Player player){
        if (draw50Open){
            player.hasAccepted50MoveDraw = true;
        }
    }

    Player getOtherPlayer(Player player){
        return player.isWhite() ? white : black;
    }
    void updateGameStatus(){
        if (isThreefoldDraw()){
            drawThreefoldOpen = true;
        } else{
            drawThreefoldOpen = false; black.hasAcceptedThreefoldDraw = false; white.hasAcceptedThreefoldDraw = false;
        }
        if (currentPlayer.hasOfferedDraw && !getOtherPlayer(currentPlayer).hasAcceptedDraw)currentPlayer.hasOfferedDraw = false;
        board.setPossibleMoves();

        //CHECKMATE
        if (board.isInCheckMate(white))status = GameStatus.WHITE_IN_CHECKMATE;
        else if (board.isInCheckMate(black))status = GameStatus.BLACK_IN_CHECKMATE;

        //TIMEOUT
        else if(white.hasTimedOut)status = GameStatus.WHITE_LOSS_BY_TIMEOUT;
        else if(black.hasTimedOut)status = GameStatus.BLACK_LOSS_BY_TIMEOUT;

        //DRAW
        else if (draw50count >= 50 && currentPlayer.hasAccepted50MoveDraw)status = GameStatus.DRAW_50_MOVES;
        else if (drawThreefoldOpen && currentPlayer.hasAcceptedThreefoldDraw)status = GameStatus.DRAW_REPETITION;
        else if ((white.hasOfferedDraw && black.hasAcceptedDraw) || (black.hasOfferedDraw && white.hasAcceptedDraw))status = GameStatus.DRAW_AGREED;
        else if (board.isLowMaterial(white) && board.isLowMaterial(black))status = GameStatus.DRAW_LOW_MATERIAL;
        else if (currentPlayer.getPossibleMoves(board).isEmpty())status = GameStatus.DRAW_STALEMATE;

        //RESIGNED
        else if(white.hasResigned)status = GameStatus.WHITE_LOSS_BY_RESIGNATION;
        else if(black.hasResigned)status = GameStatus.BLACK_LOSS_BY_RESIGNATION;
        else status = currentPlayer.isWhite() ? GameStatus.BLACK_TO_PLAY : GameStatus.WHITE_TO_PLAY;
        gameIsOver = status.isGameOver();
    }
    void updatePlayer(){
        currentPlayer = currentPlayer.isWhite() ? black : white;
    }
    void updateDraw50Count(Move move){
        draw50count = (!board.getSquare(move.startPos).piece.pieceType.isPawn() && !move.isCapture) ? draw50count + 1 : 0;
        draw50Open = draw50count >= 50;
    }
    boolean isThreefoldDraw(){
        int count = 0;
        String current = positions.get(positions.size()-1);
        for (String position : positions)
            if (position.equals(current))count++;
        return count >= 3;
    }
}