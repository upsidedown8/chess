package game;

import game.board.Board;
import game.board.move.Move;
import game.board.move.exceptions.*;
import game.player.*;
import game.player.computer.Computer;

import java.util.ArrayList;
import java.util.List;

public class Chess {
    public Board board;
    public int totalMoves;
    public Player whitePlayer, blackPlayer;
    public GameStatus status;
    public boolean gameIsOver, draw50Open, drawThreefoldOpen;
    private boolean whiteToMove;
    int draw50count;
    List<String> positions;

    public Chess(Player whitePlayer, Player blackPlayer){
        this.whitePlayer = whitePlayer;
        this.blackPlayer = blackPlayer;
        this.board = new Board();
        reset();
    }
    public void reset(){
        board.reset();
        whitePlayer.reset();
        blackPlayer.reset();
        draw50count = 0;
        status = GameStatus.WHITE_TO_PLAY;
        positions = new ArrayList<String>();
        positions.add(board.toString());
        gameIsOver = false;
        draw50Open = false;
        drawThreefoldOpen = false;
        totalMoves = 0;
        whiteToMove = true;
        if (whitePlayer.isComputer()){
            try{
                Move computerMove = ((Computer)whitePlayer).getNextMove(board);
                move(computerMove);
            } catch (MoveException moveException){
                moveException.printStackTrace();
            }
        }
    }

    public void move(Move move) throws MoveException{
        if (!gameIsOver){
            if (move.isCapture)
                getCurrentPlayer().addEnemyPiece(board.getSquare(move.endPos).piece.pieceType);
            getCurrentPlayer().addMove(move);
            updatePlayer();
            updateDraw50Count(move);
            move.doMove(board);
            board.setEnPassantPawnVirtual(move);
            positions.add(board.toString());
            updateGameStatus();
            totalMoves++;
            if (getCurrentPlayer().isComputer()){
                Move computerMove = ((Computer)getCurrentPlayer()).getNextMove(board);
                move(computerMove);
            }
        } else throw new GameIsOverException();
    }
    public void undo() throws MoveException{
        if (!gameIsOver){
            updatePlayer();
            Move previousMove;
            try{
                previousMove = getCurrentPlayer().previousMove();
                previousMove.undoMove(board);
                if (previousMove.isCapture)
                    getCurrentPlayer().removeEnemyPiece(board.getSquare(previousMove.endPos).piece.pieceType);
                draw50count = previousMove.prevDraw50;
                board.enPassantPawnVirtual = previousMove.prevEnPassantPawnVirtual;
                positions.remove(positions.size()-1);
                updateGameStatus();
                totalMoves--;
            } catch (MoveException moveException){
                updatePlayer();
                throw moveException;
            }
        } else throw new GameIsOverException();
    }
    public void redo() throws MoveException{
        if (!gameIsOver) {
            Move nextMove;
            nextMove = getCurrentPlayer().nextMove();
            if (nextMove.isCapture)
                getCurrentPlayer().addEnemyPiece(board.getSquare(nextMove.endPos).piece.pieceType);
            getCurrentPlayer().addMove(nextMove);
            updatePlayer();
            updateDraw50Count(nextMove);
            nextMove.doMove(board);
            board.setEnPassantPawnVirtual(nextMove);
            positions.add(board.toString());
            updateGameStatus();
            totalMoves++;
        } else throw new GameIsOverException();
    }
    public Move getMove(int start, int end) throws MoveException {
        if (!Board.isOnBoard(start) || !Board.isOnBoard(end)) throw new TryToMoveSquareNotOnBoardException();
        if (!board.getSquare(start).isOccupied()) throw new EmptySquareSelectedException();
        if (start == end) throw new TryToMovePieceToSameSquareException();
        if (board.getSquare(start).piece.colour != getCurrentPlayer().colour) throw new TryToMoveEnemyPieceException();
        Move yourMove = null;
        for (Move move : getCurrentPlayer().getPossibleMoves(board)) {
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

    public final Player getCurrentPlayer(){
        return whiteToMove ? whitePlayer : blackPlayer;
    }
    public final Player getOpponent(){
        return whiteToMove ? blackPlayer : whitePlayer;
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
        if (!gameIsOver && getCurrentPlayer() == player) {
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

    final Player getOtherPlayer(Player player){
        return player.isWhite() ? whitePlayer : blackPlayer;
    }
    void updateGameStatus(){
        if (isThreefoldDraw()){
            drawThreefoldOpen = true;
        } else{
            drawThreefoldOpen = false; blackPlayer.hasAcceptedThreefoldDraw = false; whitePlayer.hasAcceptedThreefoldDraw = false;
        }
        if (getCurrentPlayer().hasOfferedDraw && !getOtherPlayer(getCurrentPlayer()).hasAcceptedDraw)getCurrentPlayer().hasOfferedDraw = false;
        board.setPossibleMoves();

        //CHECKMATE
        if (board.isInCheckMate(whitePlayer))status = GameStatus.WHITE_IN_CHECKMATE;
        else if (board.isInCheckMate(blackPlayer))status = GameStatus.BLACK_IN_CHECKMATE;

        //TIMEOUT
        else if(whitePlayer.hasTimedOut)status = GameStatus.WHITE_LOSS_BY_TIMEOUT;
        else if(blackPlayer.hasTimedOut)status = GameStatus.BLACK_LOSS_BY_TIMEOUT;

        //DRAW
        else if (draw50count >= 50 && getCurrentPlayer().hasAccepted50MoveDraw)status = GameStatus.DRAW_50_MOVES;
        else if (drawThreefoldOpen && getCurrentPlayer().hasAcceptedThreefoldDraw)status = GameStatus.DRAW_REPETITION;
        else if ((whitePlayer.hasOfferedDraw && blackPlayer.hasAcceptedDraw) || (blackPlayer.hasOfferedDraw && whitePlayer.hasAcceptedDraw))status = GameStatus.DRAW_AGREED;
        else if (board.isLowMaterial(whitePlayer) && board.isLowMaterial(blackPlayer))status = GameStatus.DRAW_LOW_MATERIAL;
        else if (getCurrentPlayer().getPossibleMoves(board).isEmpty())status = GameStatus.DRAW_STALEMATE;

        //RESIGNED
        else if(whitePlayer.hasResigned)status = GameStatus.WHITE_LOSS_BY_RESIGNATION;
        else if(blackPlayer.hasResigned)status = GameStatus.BLACK_LOSS_BY_RESIGNATION;
        else status = getCurrentPlayer().isWhite() ? GameStatus.BLACK_TO_PLAY : GameStatus.WHITE_TO_PLAY;
        gameIsOver = status.isGameOver();
    }
    void updatePlayer(){
        whiteToMove = !whiteToMove;
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