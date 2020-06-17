package com.example.chessclassic.game.player;

public enum Colour {
    WHITE {
        @Override
        public String toString(){
            return "White";
        }
        public boolean isWhite() { return true; }
        public boolean isBlack() { return false; }
    },
    BLACK{
        @Override
        public String toString(){
            return "Black";
        }
        public boolean isWhite() { return false; }
        public boolean isBlack() { return true; }
    };
    public abstract boolean isWhite();
    public abstract boolean isBlack();
    public Colour reverse(){
        return this == WHITE ? BLACK : WHITE;
    }
}
