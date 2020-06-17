package game.board.piece;

public enum PieceType {
    PAWN{
        @Override
        public String toString(){
            return "Pawn";
        }
        public char getCode(){
            return 'P';
        }
        public int getValue(){
            return 1;
        }
    },
    KNIGHT{
        @Override
        public String toString(){
            return "Knight";
        }
        public char getCode(){
            return 'N';
        }
        public int getValue(){
            return 3;
        }
    },
    BISHOP{
        @Override
        public String toString(){
            return "Bishop";
        }
        public char getCode(){
            return 'B';
        }
        public int getValue(){
            return 3;
        }
    },
    ROOK{
        @Override
        public String toString(){
            return "Rook";
        }
        public char getCode(){
            return 'R';
        }
        public int getValue(){
            return 5;
        }
    },
    QUEEN{
        @Override
        public String toString(){
            return "Queen";
        }
        public char getCode(){
            return 'Q';
        }
        public int getValue(){
            return 9;
        }
    },
    KING{
        @Override
        public String toString(){
            return "King";
        }
        public char getCode(){
            return 'K';
        }
        public int getValue(){
            return 100;
        }
    },
    EMPTY{
        @Override
        public String toString(){
            return "Empty";
        }
        public char getCode(){
            return '#';
        }
        public int getValue(){
            return 0;
        }
    };

    public abstract char getCode();
    public abstract int getValue();

    public boolean isPawn(){
        return this == PAWN;
    }
    public boolean isBishop(){
        return this == BISHOP;
    }
    public boolean isKnight(){
        return this == KNIGHT;
    }
    public boolean isRook(){
        return this == ROOK;
    }
    public boolean isKing(){
        return this == KING;
    }
    public boolean isQueen(){
        return this == QUEEN;
    }

    public boolean isEmpty() { return this == EMPTY; }
}
