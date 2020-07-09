package game.board.move;

public enum MoveInfo {
    SUCCESS{
        @Override
        public String toString(){
            return "Success";
        }
        public boolean isSuccess(){
            return true;
        }
    },
    ILLEGAL_MOVE{
        @Override
        public String toString(){
            return "Illegal move";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_MOVE_SQUARE_NOT_ON_BOARD{
        @Override
        public String toString(){
            return "Selected square not on board";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_MOVE_EMPTY_SQUARE{
        @Override
        public String toString(){
            return "Selected square does not contain a piece";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_MOVE_ENEMY_PIECE{
        @Override
        public String toString(){
            return "Selected square contains enemy piece";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_TAKE_FRIENDLY_PIECE{
        @Override
        public String toString(){
            return "Selected destination contains a friendly piece";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_UNDO_BEFORE_MOVE_MADE{
        @Override
        public String toString(){
            return "Trying to undo a move before that move has been made";
        }
        public boolean isSuccess(){
            return false;
        }
    },
    TRY_TO_REDO_BEFORE_MOVE_MADE{
        @Override
        public String toString(){
            return "Trying to redo a move before that move has been made";
        }
        public boolean isSuccess(){
            return false;
        }
    };

    public abstract boolean isSuccess();
}