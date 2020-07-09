package com.example.chessclassic.game.board.move;

import com.example.chessclassic.game.board.Board;

public class MoveUtils {
    public static int findRank(int position){
        return (position - findFile(position)) / 8;
    }
    public static int findFile(int position){
        return position % 8;
    }
    public static int getPositionFromAlgebraic(String algebraic){
        int rankIndex = Integer.parseInt(algebraic.substring(1, 2));
        if (rankIndex < 1 || rankIndex > 8) return -1;
        int rank = 8 - rankIndex;
        int file = Character.toLowerCase(algebraic.charAt(0)) - 97;
        if (file < 0 || file > 7) return -1;
        return rank*8 + file;
    }
    public static String getAlgebraicPosition(int position){
        if (!Board.isOnBoard(position)) throw new IllegalArgumentException("position was not on board");
        int rank = findRank(position);
        int file = findFile(position);
        return Character.toString((char)(file+97)) + (8 - rank);
    }
}