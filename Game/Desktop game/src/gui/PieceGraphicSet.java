package gui;

public enum PieceGraphicSet {
    STANDARD{
        public String toString(){
            return "Standard";
        }
        public String filePath(boolean isWhite){
            return isWhite ? "standard/w/" : "standard/b/";
        }
    },
    LETTERS{
        public String toString(){
            return "Letters";
        }
        public String filePath(boolean isWhite){
            return isWhite ? "letters/w/" : "letters/b/";
        }
    };
    public abstract String toString();
    public abstract String filePath(boolean isWhite);
}