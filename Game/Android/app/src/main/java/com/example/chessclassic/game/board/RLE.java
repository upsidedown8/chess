package com.example.chessclassic.game.board;

public class RLE {
    public static String encode(String text){
        StringBuilder sb = new StringBuilder();
        for (int textIndex = 0; textIndex < text.length(); textIndex++){
            int runLength = 1;
            while (textIndex+1 < text.length() && text.charAt(textIndex) == text.charAt(textIndex+1)){
                runLength++; textIndex++;
            }
            if (runLength != 1) sb.append(runLength);
            sb.append(text.charAt(textIndex));
        }
        return sb.toString();
    }
    public static String decode(String text){
        StringBuilder sb = new StringBuilder();
        int textIndex = 0;
        while (textIndex < text.length()) {
            int digits = 0;
            while(Character.isDigit(text.charAt(textIndex+digits))) digits++;
            int runLength = digits > 0 ? Integer.parseInt(text.substring(textIndex, textIndex + digits)) : 1;
            for (int x = 0; x < runLength; x++) sb.append(text.charAt(textIndex + digits));
            textIndex += digits + 1;
        }
        return sb.toString();
    }
}
