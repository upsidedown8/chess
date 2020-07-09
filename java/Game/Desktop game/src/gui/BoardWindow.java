package gui;

import game.Chess;
import game.board.Board;
import game.board.RLE;
import game.board.Square;
import game.board.move.EnPassant;
import game.board.move.Move;
import game.board.move.MoveUtils;
import game.board.move.Promotion;
import game.board.move.exceptions.MoveException;
import game.board.piece.Piece;
import game.player.*;
import game.player.computer.Computer;
import game.player.computer.Setting;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

import static javax.swing.SwingUtilities.isLeftMouseButton;
import static javax.swing.SwingUtilities.isRightMouseButton;

public class BoardWindow {
    // constants
    private static final Dimension OUTER_FRAME_DIMENSION = new Dimension(600, 600);
    private static final Dimension BOARD_PANEL_DIMENSION = new Dimension(400, 400);
    private static final Dimension SQUARE_PANEL_DIMENSION = new Dimension(50, 50);
    private static final String ICON_GRAPHIC_PATH = "src/gui/images/icon/";
    private static final String PIECE_GRAPHIC_PATH = "src/gui/images/piece/";
    private static final String MOVE_INDICATOR_GRAPHIC_PATH = "src/gui/images/moveindicator/";
    private static final String GRAPHIC_EXTENSION = ".png";

    //window
    private final JFrame frame;
    private final BoardPanel boardPanel;

    //graphics
    private PieceGraphicSet pieceGraphicSet;
    private BoardGraphicSet boardGraphicSet;
    private int moveStart, moveEnd;
    private int lastMoveStart, lastMoveEnd;
    private BoardOrientation boardOrientation;
    private boolean highlightLegalMoves, highlightLastMove, highlightSelectedPiece, flipBoardEveryTurn, displayIndicatorsAbovePiece;

    //game
    private Chess chess;
    private Player whitePlayer, blackPlayer;


    public BoardWindow(){
        //game
        this.whitePlayer = new WhitePlayer();
        this.blackPlayer = new BlackPlayer();
        this.chess = new Chess(whitePlayer, blackPlayer);

        //graphics
        this.pieceGraphicSet = PieceGraphicSet.STANDARD;
        this.boardGraphicSet = BoardGraphicSet.GREEN_AND_CREAM;
        this.moveStart = -1;
        this.moveEnd = -1;
        this.lastMoveStart = -1;
        this.lastMoveEnd = -1;
        this.boardOrientation = BoardOrientation.NORMAL;
        this.highlightSelectedPiece = true;
        this.highlightLegalMoves = true;
        this.highlightLastMove = true;
        this.flipBoardEveryTurn = false;
        this.displayIndicatorsAbovePiece = false;

        //window
        this.frame = new JFrame("Chess board");
        this.frame.setLayout(new BorderLayout());
        JMenuBar menuBar = createMenuBar();
        this.frame.setJMenuBar(menuBar);
        this.frame.setSize(OUTER_FRAME_DIMENSION);
        this.boardPanel = new BoardPanel();
        this.frame.add(boardPanel, BorderLayout.CENTER);
        this.frame.setIconImage(getAppIcon());
        this.frame.setResizable(false);
        this.frame.setVisible(true);
        this.frame.validate();
        boardPanel.drawSquares();
    }

    // GUI Components
    private BufferedImage resizeImg(BufferedImage img, int newW, int newH) {
        int w = img.getWidth();
        int h = img.getHeight();
        BufferedImage dimg = new BufferedImage(newW, newH, img.getType());
        Graphics2D g = dimg.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
                RenderingHints.VALUE_INTERPOLATION_BILINEAR);
        g.drawImage(img, 0, 0, newW, newH, 0, 0, w, h, null);
        g.dispose();
        return dimg;
    }
    private JMenuBar createMenuBar() {
        final JMenuBar menuBar = new JMenuBar();
        menuBar.add(createFileBar());
        menuBar.add(createPreferencesBar());
        menuBar.add(createOptionsBar());
        return menuBar;
    }
    private JMenu createFileBar() {
        final JMenu fileMenu = new JMenu("File");

        final JMenuItem openPGN = new JMenuItem("Load PGN File");
        openPGN.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("Load PGN File");
            }
        });
        fileMenu.add(openPGN);

        final JMenuItem exit = new JMenuItem("Exit");
        exit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                System.out.println("Exit");
                frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
            }
        });
        fileMenu.add(exit);

        return fileMenu;
    }
    private JMenu createPreferencesBar(){
        final JMenu  preferencesMenu = new JMenu("Preferences");

        /* CHANGE PIECE SET */
        preferencesMenu.add(createPieceGraphicsSetSubMenu());

        /* CHANGE BOARD COLOURS */
        preferencesMenu.add(createBoardGraphicsSetSubMenu());


        preferencesMenu.addSeparator();


        /* CHANGE ORIENTATION SETTINGS */
        final JMenuItem changeOrientationSingleTime = new JMenuItem("Flip board");
        changeOrientationSingleTime.addActionListener(actionEvent -> {
            boardOrientation = boardOrientation.flip();
            boardPanel.drawSquares();
        });
        preferencesMenu.add(changeOrientationSingleTime);

        /* CHANGE ORIENTATION SETTINGS */
        final JCheckBoxMenuItem changeOrientationEveryTurn = new JCheckBoxMenuItem("Flip board every turn");
        changeOrientationEveryTurn.addActionListener(actionEvent -> {
            flipBoardEveryTurn = changeOrientationEveryTurn.getState();
            boardPanel.drawSquares();
        });
        changeOrientationEveryTurn.setState(flipBoardEveryTurn);
        preferencesMenu.add(changeOrientationEveryTurn);


        preferencesMenu.addSeparator();


        /* CHANGE LEGAL MOVE HIGHLIGHTING */
        final JCheckBoxMenuItem highlightLegalMovesCheckBox = new JCheckBoxMenuItem("Highlight legal moves");
        highlightLegalMovesCheckBox.addActionListener(actionEvent -> highlightLegalMoves = highlightLegalMovesCheckBox.isSelected());
        highlightLegalMovesCheckBox.setState(highlightLegalMoves);
        preferencesMenu.add(highlightLegalMovesCheckBox);

        /* CHANGE LAST MOVE HIGHLIGHTING */
        final JCheckBoxMenuItem highlightLastMoveCheckBox = new JCheckBoxMenuItem("Highlight most recent move");
        highlightLastMoveCheckBox.addActionListener(actionEvent -> highlightLastMove = highlightLastMoveCheckBox.isSelected());
        highlightLastMoveCheckBox.setState(highlightLastMove);
        preferencesMenu.add(highlightLastMoveCheckBox);

        /* CHANGE SELECTED PIECE HIGHLIGHTING */
        final JCheckBoxMenuItem highlightSelectedPieceCheckBox = new JCheckBoxMenuItem("Highlight selected piece");
        highlightSelectedPieceCheckBox.addActionListener(actionEvent -> highlightSelectedPiece = highlightSelectedPieceCheckBox.isSelected());
        highlightSelectedPieceCheckBox.setState(highlightSelectedPiece);
        preferencesMenu.add(highlightSelectedPieceCheckBox);

        /* CHANGE INDICATOR LAYERING PROPERTIES */
        final JCheckBoxMenuItem displayIndicatorsAbovePieceCheckBox = new JCheckBoxMenuItem("Display move indicator over pieces");
        displayIndicatorsAbovePieceCheckBox.addActionListener(actionEvent -> displayIndicatorsAbovePiece = displayIndicatorsAbovePieceCheckBox.isSelected());
        displayIndicatorsAbovePieceCheckBox.setState(displayIndicatorsAbovePiece);
        preferencesMenu.add(displayIndicatorsAbovePieceCheckBox);

        return preferencesMenu;
    }
    private JMenu createOptionsBar(){
        final JMenu optionsMenu = new JMenu("Options");

        final JMenuItem resetGame = new JMenuItem("Reset game");
        resetGame.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                chess.reset();
                boardPanel.boardSquares.get(lastMoveStart).setBackgroundColour();
                boardPanel.boardSquares.get(lastMoveEnd).setBackgroundColour();
                resetMoveVariables();
                boardPanel.drawSquares();
                updateConsoleBoard();
            }
        });
        optionsMenu.add(resetGame);

        final JMenuItem undoMove = new JMenuItem("Undo");
        undoMove.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                try{
                    chess.undo();
                    boardPanel.drawSquares();
                    updateConsoleBoard();
                } catch (MoveException moveException){
                    moveException.printStackTrace();
                }
            }
        });
        optionsMenu.add(undoMove);

        final JMenuItem redoMove = new JMenuItem("Redo");
        redoMove.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                try {
                    chess.redo();
                    boardPanel.drawSquares();
                    updateConsoleBoard();
                } catch (MoveException moveException){
                    moveException.printStackTrace();
                }
            }
        });
        optionsMenu.add(redoMove);

        return optionsMenu;
    }
    private JMenu createBoardGraphicsSetSubMenu(){
        final JMenu changeBoardSet = new JMenu("Board set");

        JRadioButton radioButtonBlackAndWhite = new JRadioButton(BoardGraphicSet.BLACK_AND_WHITE.toString());
        radioButtonBlackAndWhite.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                boardGraphicSet = BoardGraphicSet.BLACK_AND_WHITE;
                boardPanel.drawSquares();
            }
        });
        changeBoardSet.add(radioButtonBlackAndWhite);

        JRadioButton radioButtonGreyAndWhite = new JRadioButton(BoardGraphicSet.GREY_AND_WHITE.toString());
        radioButtonGreyAndWhite.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                boardGraphicSet = BoardGraphicSet.GREY_AND_WHITE;
                boardPanel.drawSquares();
            }
        });
        changeBoardSet.add(radioButtonGreyAndWhite);

        JRadioButton radioButtonGreenAndCream = new JRadioButton(BoardGraphicSet.GREEN_AND_CREAM.toString());
        radioButtonGreenAndCream.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                boardGraphicSet = BoardGraphicSet.GREEN_AND_CREAM;
                boardPanel.drawSquares();
            }
        });
        radioButtonGreenAndCream.setSelected(true);
        changeBoardSet.add(radioButtonGreenAndCream);

        ButtonGroup radioButtonGroup = new ButtonGroup();
        radioButtonGroup.add(radioButtonBlackAndWhite);
        radioButtonGroup.add(radioButtonGreenAndCream);
        radioButtonGroup.add(radioButtonGreyAndWhite);

        return changeBoardSet;
    }
    private JMenu createPieceGraphicsSetSubMenu(){
        final JMenu changePieceSet = new JMenu("Piece set");

        JRadioButton radioButtonLetters = new JRadioButton(PieceGraphicSet.LETTERS.toString());
        radioButtonLetters.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                pieceGraphicSet = PieceGraphicSet.LETTERS;
                boardPanel.drawSquares();
            }
        });
        radioButtonLetters.setSelected(false);
        changePieceSet.add(radioButtonLetters);

        JRadioButton radioButtonStandard = new JRadioButton(PieceGraphicSet.STANDARD.toString());
        radioButtonStandard.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                pieceGraphicSet = PieceGraphicSet.STANDARD;
                boardPanel.drawSquares();
            }
        });
        radioButtonStandard.setSelected(true);
        changePieceSet.add(radioButtonStandard);

        ButtonGroup radioButtonGroup = new ButtonGroup();
        radioButtonGroup.add(radioButtonLetters);
        radioButtonGroup.add(radioButtonStandard);

        return changePieceSet;
    }
    private Image getAppIcon(){
        String path = ICON_GRAPHIC_PATH + "app-icon.png";
        return new ImageIcon(path).getImage();
    }

    //Update GUI
    public void updateConsoleBoard(){
        System.out.println(chess.getCurrentPlayer() == chess.whitePlayer ? "White to move" : "Black to move");
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

    //Start game
    public void startConsoleGame(){
        Scanner scanner = new Scanner(System.in);
        while(!chess.gameIsOver) {
            if (!chess.getCurrentPlayer().isComputer()){
                try{
                    updateConsoleBoard();
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
                        assignLastMove(move);
                        boardPanel.drawSquares();
                        System.out.println(move.toString());
                    }
                }catch (MoveException moveException){
                    System.out.println(moveException.toString());
                }
            }
        }
    }

    //Move functionality
    private void resetMoveVariables(){
        if (moveStart != -1)
            boardPanel.removeIndicatorsForPossibleMoves(moveStart);
        moveStart = -1;
        moveEnd = -1;
    }
    private void assignLastMove(Move move){
        if (highlightLastMove){
            lastMoveStart = move.startPos;
            if (move instanceof EnPassant){
                EnPassant enPassantMove = (EnPassant) move;
                lastMoveEnd = enPassantMove.virtualPawnPos;
            } else {
                lastMoveEnd = move.endPos;
            }
        } else{
            lastMoveStart = -1;
            lastMoveEnd = -1;
        }
    }

    //Board GUI components
    private class BoardPanel extends JPanel{
        List<SquarePanel> boardSquares;

        BoardPanel(){
            super(new GridLayout(8, 8));
            this.boardSquares = new ArrayList<>();
            for (int i = 0; i < Board.NUMBER_OF_SQUARES; i++){
                final SquarePanel squarePanel = new SquarePanel(this, i);
                this.boardSquares.add(squarePanel);
                add(squarePanel);
            }
            setPreferredSize(BOARD_PANEL_DIMENSION);
            validate();
        }
        public void addIndicatorsForPossibleMoves(int start){
            if (highlightLegalMoves){
                for (final Move possibleMove : chess.getCurrentPlayer().getPossibleMoves(chess.board)){
                    if (possibleMove.startPos == start){
                        int endPos;
                        if (possibleMove instanceof EnPassant){
                            EnPassant enPassantMove = (EnPassant)possibleMove;
                            endPos = enPassantMove.virtualPawnPos;
                        } else{
                            endPos = possibleMove.endPos;
                        }
                        boardSquares.get(endPos).addMoveIndicator(chess.board);
                    }
                }
            }
        }
        public void removeIndicatorsForPossibleMoves(int start){
            for (final Move possibleMove : chess.getCurrentPlayer().getPossibleMoves(chess.board)){
                if (possibleMove.startPos == start){
                    boardSquares.get(possibleMove.endPos).removeMoveIndicator(chess.board);
                }
            }
        }
        public void drawSquares(){
            removeAll();
            for (SquarePanel squarePanel : boardOrientation.getBoardSquares(boardSquares)){
                squarePanel.drawSquare(chess.board);
                add(squarePanel);
            }
            validate();
            repaint();
        }
    }
    private class SquarePanel extends JPanel{
        private final int id;
        private final Colour colour;
        private JLabel piece;

        SquarePanel(final BoardPanel boardPanel, final int id){
            super(new GridBagLayout());
            this.id = id;
            this.colour = getColour();
            setPreferredSize(SQUARE_PANEL_DIMENSION);
            setBackgroundColour();
            addMouseListener(new MouseListener() {
                @Override
                public void mouseClicked(final MouseEvent mouseEvent) {
                    if (!chess.getCurrentPlayer().isComputer()){
                        if (isRightMouseButton(mouseEvent)){
                            resetMoveVariables();
                        } else if (isLeftMouseButton(mouseEvent)){
                            // first click
                            if (moveStart == -1) {
                                moveStart = id;
                                Square startSquare = chess.board.getSquare(id);
                                if (chess.board.getSquare(moveStart).isOccupied()) {
                                    if (chess.getCurrentPlayer().colour.equals(startSquare.piece.colour)){
                                        boardPanel.addIndicatorsForPossibleMoves(id);
                                    }
                                    if (highlightSelectedPiece)
                                        setHighlightColour();
                                } else{
                                    resetMoveVariables();
                                    if (id != lastMoveStart && id != lastMoveEnd)
                                        setBackgroundColour();
                                }
                            } else{
                                //second click
                                moveEnd = id;
                                try{
                                    final Move move = chess.getMove(moveStart, moveEnd);
                                    chess.move(move);
                                    resetMoveVariables();
                                    assignLastMove(move);
                                    if (flipBoardEveryTurn)
                                        boardOrientation = boardOrientation.flip();
                                    boardPanel.drawSquares();
                                    updateConsoleBoard();
                                } catch (MoveException moveException){
                                    System.out.println(moveException.toString());
                                    if (moveStart != lastMoveStart && moveStart != lastMoveEnd)
                                        boardPanel.boardSquares.get(moveStart).setBackgroundColour();
                                    if (id != lastMoveStart && id != lastMoveEnd)
                                        setBackgroundColour();
                                    resetMoveVariables();
                                }
                            }
                        }
                    }
                }

                @Override
                public void mousePressed(MouseEvent mouseEvent) {

                }

                @Override
                public void mouseReleased(MouseEvent mouseEvent) {

                }

                @Override
                public void mouseEntered(MouseEvent mouseEvent) {

                }

                @Override
                public void mouseExited(MouseEvent mouseEvent) {

                }
            });
            setVisible(true);
            validate();
        }

        public void drawSquare(Board board){
            if (id == lastMoveStart || id == lastMoveEnd)
                setHighlightColour();
            else
                setBackgroundColour();
            setPiece(board, getWidth());
            validate();
            repaint();
        }
        public void setPiece(Board board, int size) {
            removeAll();
            final Square square = board.getSquare(id);
            if (square.isOccupied()) {
                try {
                    BufferedImage myPicture = ImageIO.read(new File(
                            PIECE_GRAPHIC_PATH +
                                    pieceGraphicSet.filePath(square.piece.colour.isWhite()) +
                                    square.piece.pieceType.toString().toLowerCase() +
                                    GRAPHIC_EXTENSION));
                    piece = new JLabel(new ImageIcon(resizeImg(myPicture, size, size)));
                    this.add(piece);
                } catch (IOException e) { e.printStackTrace(); }
            }
            revalidate();
            repaint();
        }
        public void addMoveIndicator(Board board){
            removeAll();
            try {
                BufferedImage myPicture = ImageIO.read(new File(MOVE_INDICATOR_GRAPHIC_PATH + boardGraphicSet.filePath(colour.isWhite()) + GRAPHIC_EXTENSION));
                JLabel jlabel = new JLabel(new ImageIcon(resizeImg(myPicture, getWidth(), getHeight())));
                jlabel.setBounds(0, 0, getWidth(), getHeight());
                if (board.getSquare(id).isOccupied()){
                    if (displayIndicatorsAbovePiece) {
                        piece.add(jlabel);
                        this.add(piece);
                    } else{
                        jlabel.add(piece);
                        this.add(jlabel);
                    }
                } else{
                    this.add(jlabel);
                }
            } catch (IOException e) { e.printStackTrace(); }
            revalidate();
            repaint();
        }
        public void removeMoveIndicator(Board board){
            setPiece(board, getWidth());
        }
        public void setHighlightColour() {
            setBackground(boardGraphicSet.highlightColour(colour));
        }
        public void setBackgroundColour() {
            setBackground(boardGraphicSet.backgroundColour(colour));
        }
        private Colour getColour(){
            if (id % 16 < 8)
                return id % 2 == 0 ? Colour.WHITE : Colour.BLACK;
            return id % 2 == 0 ? Colour.BLACK : Colour.WHITE;
        }
    }
    private enum BoardOrientation {
        NORMAL{
            public BoardOrientation flip(){
                return FLIPPED;
            }
            public List<SquarePanel> getBoardSquares(final List<SquarePanel> boardSquares){
                return boardSquares;
            }
        },
        FLIPPED{
            public BoardOrientation flip(){
                return NORMAL;
            }
            public List<SquarePanel> getBoardSquares(final List<SquarePanel> boardSquares){
                List<SquarePanel> reversed = new ArrayList<>(boardSquares);
                Collections.reverse(reversed);
                return reversed;
            }
        };

        public abstract BoardOrientation flip();
        public abstract List<SquarePanel> getBoardSquares(final List<SquarePanel> boardSquares);
    }
}