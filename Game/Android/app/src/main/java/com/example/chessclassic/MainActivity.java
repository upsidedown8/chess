package com.example.chessclassic;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TableLayout;
import android.widget.TableRow;

import com.example.chessclassic.game.board.RLE;
import com.example.chessclassic.game.Chess;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.board.move.exceptions.MoveException;
import com.example.chessclassic.game.player.WhitePlayer;
import com.example.chessclassic.game.player.BlackPlayer;

public class MainActivity extends AppCompatActivity {
    private TableLayout boardView;
    private boolean pieceSelected, destinationSelected;

    private int start, finish;

    private Chess chessGame;
    WhitePlayer whitePlayer;
    BlackPlayer blackPlayer;

    private boolean isBlack(Drawable drawable){
        return (drawable == getDrawable(R.drawable.bpawn) ||
                drawable == getDrawable(R.drawable.bknight) ||
                drawable == getDrawable(R.drawable.bbishop) ||
                drawable == getDrawable(R.drawable.brook) ||
                drawable == getDrawable(R.drawable.bqueen) ||
                drawable == getDrawable(R.drawable.bking));
    }
    private boolean isWhite(Drawable drawable){
        return (drawable == getDrawable(R.drawable.wpawn) ||
                drawable == getDrawable(R.drawable.wknight) ||
                drawable == getDrawable(R.drawable.wbishop) ||
                drawable == getDrawable(R.drawable.wrook) ||
                drawable == getDrawable(R.drawable.wqueen) ||
                drawable == getDrawable(R.drawable.wking));
    }
    private ImageView getSquare(int rank, int file){
        TableRow row = (TableRow) boardView.getChildAt(rank);
        return (ImageView)row.getChildAt(file);
    }
    private void setBoard(){
        String data = RLE.decode(chessGame.board.toString());
        for (int rank = 0; rank < 8; rank++){
            for (int file = 0; file < 8; file++){
                int pos = rank * 8 + file;
                char c = data.charAt(pos);
                ImageView square = getSquare(rank, file);
                if (c == '#') square.setAlpha(0f);
                else{
                    square.setAlpha(1f);
                    if (Character.isUpperCase(c)){
                        switch(c){
                            case 'P': square.setImageResource(R.drawable.wpawn); break;
                            case 'N': square.setImageResource(R.drawable.wknight); break;
                            case 'B': square.setImageResource(R.drawable.wbishop); break;
                            case 'R': square.setImageResource(R.drawable.wrook); break;
                            case 'Q': square.setImageResource(R.drawable.wqueen); break;
                            case 'K': square.setImageResource(R.drawable.wking); break;
                            default: break;
                        }
                    }
                    else{
                        switch(c){
                            case 'p': square.setImageResource(R.drawable.bpawn); break;
                            case 'n': square.setImageResource(R.drawable.bknight); break;
                            case 'b': square.setImageResource(R.drawable.bbishop); break;
                            case 'r': square.setImageResource(R.drawable.brook); break;
                            case 'q': square.setImageResource(R.drawable.bqueen); break;
                            case 'k': square.setImageResource(R.drawable.bking); break;
                        }
                    }
                }
                switch(c){
                    case '#':  break;
                    case 'p': square.setImageResource(R.drawable.bpawn);
                }
            }
        }
    }
    private void createBoard(){
        View.OnClickListener listener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                btnClick(view);
            }
        };
        TableRow.LayoutParams params = new TableRow.LayoutParams(
                TableRow.LayoutParams.MATCH_PARENT,
                TableRow.LayoutParams.MATCH_PARENT,
                1.0f
        );
        for (int rank = 0; rank < 8; rank++){
            TableRow row = (TableRow) boardView.getChildAt(rank);
            for (int file = 0; file < 8; file++){
                ImageView square = new ImageView(this);
                square.setOnClickListener(listener);
                square.setLayoutParams(params);
                square.setImageResource(R.drawable.wpawn);
                square.setAlpha(0f);
                row.addView(square);
            }
        }
    }
    private void btnClick(View view){
        TableRow row = (TableRow)view.getParent();
        int rank = boardView.indexOfChild(row);
        int file = row.indexOfChild(view);
        int position = rank * 8 + file;

        Log.i("Square clicked", String.format("(%d, %d) - %d", rank, file, position));

        ImageView square = getSquare(rank, file);
        boolean squareIsOccupied = square.getAlpha() == 1f;
        boolean squareIsWhite = isWhite(square.getDrawable());

        if (squareIsOccupied){
            if (squareIsWhite){
                if (chessGame.currentPlayer.isWhite()) {
                    pieceSelected = true;
                    destinationSelected = false;
                    start = position;
                }
                else{
                    if (pieceSelected){
                        destinationSelected = true;
                        finish = position;
                    }
                }
            }
            // square is black
            else {
                if (chessGame.currentPlayer.isWhite()){
                    if (pieceSelected){
                        destinationSelected = true;
                        finish = position;
                    }
                }
                else{
                    pieceSelected = true;
                    destinationSelected = false;
                    start = position;
                }
            }
        }
        else{
            if (pieceSelected){
                destinationSelected = true;
                finish = position;
            }
        }
        if (pieceSelected && destinationSelected) {
            try{
                Move move = chessGame.getMove(start, finish);
                chessGame.move(move);
                pieceSelected = false;
                destinationSelected = false;
            } catch (MoveException moveException){
                Log.i("Exception", moveException.toString());
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        chessGame = new Chess(whitePlayer, blackPlayer);
        boardView = findViewById(R.id.board);
        createBoard();
        setBoard();
        pieceSelected = false;
        destinationSelected = false;
    }

    /*
    *
    *
    private static void start(){
        WhitePlayer white = new WhitePlayer();
        BlackPlayer black = new BlackPlayer();
        Chess chess = new Chess(white, black);
        Scanner scanner = new Scanner(System.in);
        while(!chess.gameIsOver) {
            try{
                displayBoard(chess);
                String first = scanner.next();
                if (first.charAt(0) == 'u'){
                    try{
                        chess.undo();
                    }catch (MoveException moveException){
                        System.out.println(moveException.toString());
                    }
                }
                else if (first.charAt(0) == 'r'){
                    try{
                        chess.redo();
                    }catch (MoveException moveException){
                        System.out.println(moveException.toString());
                    }
                }
                else {
                    int start = MoveUtils.getPositionFromAlgebraic(first);
                    int finish = MoveUtils.getPositionFromAlgebraic(scanner.next());
                    Move move;
                    try{
                        move = chess.getMove(start, finish);
                    } catch (MoveException moveException){
                        System.out.println(moveException.toString());
                        continue;
                    }
                    if (move instanceof Promotion){
                        System.out.println("Enter piece to promote to ( Q, R, N, B )");
                        char notation = scanner.next().charAt(0);
                        Promotion promotionMove = (Promotion)move;
                        promotionMove.promotionPieceType = Piece.getPieceTypeByNotation(notation);
                        move = promotionMove;
                    }
                    chess.move(move);
                    System.out.println(move.toString());
                }
            }catch (MoveException moveException){
                System.out.println(moveException.toString());
            }

        }
    }
    *
    * */
}
