package game;

public enum GameStatus {
    WHITE_IN_CHECKMATE{
        public String toString(){
            return "Black has won by checkmate.";
        }
    },
    BLACK_IN_CHECKMATE{
        public String toString(){
            return "White has won by checkmate.";
        }
    },
    WHITE_LOSS_BY_TIMEOUT{
        public String toString(){
            return "Black has won by timeout.";
        }
    },
    BLACK_LOSS_BY_TIMEOUT{
        public String toString(){
            return "White has won by timeout.";
        }
    },
    WHITE_LOSS_BY_RESIGNATION{
        public String toString(){
            return "Black has won by resignation.";
        }
    },
    BLACK_LOSS_BY_RESIGNATION{
        public String toString(){
            return "White has won by resignation.";
        }
    },
    DRAW_STALEMATE{
        public String toString(){
            return "Draw by stalemate";
        }
    },
    DRAW_50_MOVES{
        public String toString(){
            return "No piece has been taken and no pawn moved in 50 moves, draw.";
        }
    },
    DRAW_LOW_MATERIAL{
        public String toString(){
            return "Draw by low material.";
        }
    },
    DRAW_AGREED{
        public String toString(){
            return "Draw has been agreed.";
        }
    },
    DRAW_REPETITION{
        public String toString(){
            return "Draw by threefold repetition.";
        }
    },
    WHITE_TO_PLAY{
        public String toString(){
            return "White to play.";
        }
    },
    BLACK_TO_PLAY{
        public String toString(){
            return "Black to play.";
        }
    };

    public abstract String toString();
    boolean isGameOver(){
        return !(this == WHITE_TO_PLAY || this == BLACK_TO_PLAY);
    }
}
