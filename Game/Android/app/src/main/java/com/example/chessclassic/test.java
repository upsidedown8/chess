import com.example.chessclassic.game.board.RLE;
import com.example.chessclassic.game.Chess;
import com.example.chessclassic.game.board.move.Move;
import com.example.chessclassic.game.board.move.MoveUtils;
import com.example.chessclassic.game.board.move.Promotion;
import com.example.chessclassic.game.board.move.exceptions.MoveException;
import com.example.chessclassic.game.board.piece.Piece;
import com.example.chessclassic.game.player.WhitePlayer;
import com.example.chessclassic.game.player.BlackPlayer;

import java.util.Scanner;

public class test{
    private static void displayBoard(Chess chess){
        System.out.println(chess.currentPlayer == chess.white ? "White to move" : "Black to move");
        String boardStore = RLE.decode(chess.board.toString()).replace('#', ' ');
        for (int rank = 0; rank < 8; rank++){
            System.out.print((8-rank) + "    ");
            for (int file = 0; file < 8; file++)
                System.out.print(boardStore.charAt(rank*8 + file) + "  ");
            System.out.println();
        }
        System.out.print("\n     ");
        for (int l = 0; l < 8; l++)
            System.out.print((char)(l+65) + "  ");
        System.out.println();
    }
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
    public static void main(String[] args){
        start();
    }
}