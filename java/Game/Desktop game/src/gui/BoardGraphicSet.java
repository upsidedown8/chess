package gui;

import game.player.Colour;

import java.awt.*;

public enum BoardGraphicSet {
    BLACK_AND_WHITE{
        public String filePath(boolean isWhite){
            return isWhite ? "blackandwhite/w" : "blackandwhite/b";
        }
        public String toString(){
            return "Black and White";
        }
        public Color backgroundColour(Colour colour){
            if (colour.isWhite())
                return new Color(255, 255, 255);
            return new Color(0, 0, 0);
        }
        public Color highlightColour(Colour colour){
            if (colour.isWhite())
                return new Color(246, 246, 130);
            return new Color(186, 202, 68);
        }
    },
    GREY_AND_WHITE{
        public String filePath(boolean isWhite){
            return isWhite ? "greyandwhite/w" : "greyandwhite/b";
        }
        public String toString(){
            return "Grey and White";
        }
        public Color backgroundColour(Colour colour){
            if (colour.isWhite())
                return new Color(255, 255, 255);
            return new Color(111, 111, 111);
        }
        public Color highlightColour(Colour colour){
            if (colour.isWhite())
                return new Color(139, 201, 255);
            return new Color(103, 165, 219);
        }
    },
    GREEN_AND_CREAM{
        public String filePath(boolean isWhite){
            return isWhite ? "greenandcream/w" : "greenandcream/b";
        }
        public String toString(){
        return "Green and Cream (Chess.com)";
    }
        public Color backgroundColour(Colour colour){
            if (colour.isWhite())
                return new Color(238, 238, 210);
            return new Color(118, 150, 86);
        }
        public Color highlightColour(Colour colour){
            if (colour.isWhite())
                return new Color(246, 246, 130);
            return new Color(186, 202, 68);
        }
    };

    public abstract String filePath(boolean isWhite);
    public abstract String toString();
    public abstract Color backgroundColour(Colour colour);
    public abstract Color highlightColour(Colour colour);
}
