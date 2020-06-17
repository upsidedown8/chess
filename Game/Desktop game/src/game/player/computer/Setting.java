package game.player.computer;

public enum Setting {
    EASY{
        @Override
        public String toString() {
            return "Easy";
        }
    },
    MEDIUM{
        @Override
        public String toString() {
            return "Medium";
        }
    },
    HARD{
        @Override
        public String toString() {
            return "Hard";
        }
    };
}
