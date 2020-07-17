#include "ui/chessWindow.hpp"

ChessWindow::ChessWindow() {
    setup();
}

void ChessWindow::start() {
    window = new RenderWindow(VideoMode(len, len), "Chess");
    
    bool isDrag = false;

    int primaryIdx = -1;
    int secondaryIdx = -1;
    bool primaryMouseDown = false;
    bool secondaryMouseDown = false;

    Vector2i lastPos(0,0);

    while (window->isOpen()) {
        int width = window->getSize().x;
        int height = window->getSize().y;

        Vector2i pos = Mouse::getPosition(*window);

        vector<int> highlightSquares = game.getPreviousHighlights();
        vector<int> moveIndicatorSquares;

        if (lastPos != pos && primaryMouseDown)
            isDrag = true;

        Event e;
        while (window->pollEvent(e)){
            if (e.type == Event::Closed)
                window->close();
            
            int idx = findIdxByPos(pos, width, height);

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    if (primaryIdx != -1) {
                        if (idx == -1) {
                            primaryIdx = -1;
                        } else {
                            secondaryMouseDown = true;
                            secondaryIdx = idx;
                        }
                    } else if (idx != -1) {
                        primaryMouseDown = true;
                        primaryIdx = idx;
                    }
                }
            }
            else if (e.type == Event::MouseButtonReleased) {
                if (e.mouseButton.button == Mouse::Left) {
                    if (secondaryMouseDown) {
                        secondaryMouseDown = false;
                        if (isDrag) {
                            isDrag = false;
                            if (idx != -1)
                                secondaryIdx = idx;
                        }
                    } else if (primaryMouseDown) {
                        primaryMouseDown = false;
                        if (isDrag) {
                            isDrag = false;
                            if (idx != -1)
                                secondaryIdx = idx;
                        }
                    }
                } else if (e.mouseButton.button == Mouse::Right) {
                    isDrag = false;
                    primaryIdx = -1;
                    secondaryIdx = -1;
                    primaryMouseDown = false;
                }
            }
        }

        if (((!isDrag && !primaryMouseDown) || isDrag) && primaryIdx != -1) {
            highlightSquares.push_back(primaryIdx);
        }

        if (primaryIdx != -1) {
            if (secondaryIdx != -1) {
                game.makeMove(primaryIdx, secondaryIdx);
                primaryIdx = -1;
                secondaryIdx = -1;
            } else {
                moveIndicatorSquares = game.getMoveIndicators(primaryIdx);
            }
        }

        window->clear();
        drawBoard(highlightSquares);
        drawLabels();
        drawMoveIndicators(moveIndicatorSquares);
        drawPieces(isDrag, primaryIdx, pos);
        window->display();
        lastPos = pos;
    }
}

void ChessWindow::setup() {
    game = *new Game();
    
    // load font
    labelFont.loadFromFile("assets/fonts/ubuntu.ttf");

    // load piece graphics
    wpieces[game.moveCalculator.board.getIdx(WPawn)  ].loadFromFile("assets/pieces/w/pawn.png");
    wpieces[game.moveCalculator.board.getIdx(WKnight)].loadFromFile("assets/pieces/w/knight.png");
    wpieces[game.moveCalculator.board.getIdx(WBishop)].loadFromFile("assets/pieces/w/bishop.png");
    wpieces[game.moveCalculator.board.getIdx(WRook)  ].loadFromFile("assets/pieces/w/rook.png");
    wpieces[game.moveCalculator.board.getIdx(WQueen) ].loadFromFile("assets/pieces/w/queen.png");
    wpieces[game.moveCalculator.board.getIdx(WKing)  ].loadFromFile("assets/pieces/w/king.png");

    bpieces[game.moveCalculator.board.getIdx(BPawn)  ].loadFromFile("assets/pieces/b/pawn.png");
    bpieces[game.moveCalculator.board.getIdx(BKnight)].loadFromFile("assets/pieces/b/knight.png");
    bpieces[game.moveCalculator.board.getIdx(BBishop)].loadFromFile("assets/pieces/b/bishop.png");
    bpieces[game.moveCalculator.board.getIdx(BRook)  ].loadFromFile("assets/pieces/b/rook.png");
    bpieces[game.moveCalculator.board.getIdx(BQueen) ].loadFromFile("assets/pieces/b/queen.png");
    bpieces[game.moveCalculator.board.getIdx(BKing)  ].loadFromFile("assets/pieces/b/king.png");

    whiteHighlight  = *new Color(0xf6, 0xf6, 0x82);
    whiteBackground = *new Color(0xee, 0xee, 0xd2);
    blackHighlight  = *new Color(0xba, 0xca, 0x44);
    blackBackground = *new Color(0x77, 0x96, 0x56);
    whiteIndicator  = *new Color(0xbe, 0xbe, 0xa8);
    blackIndicator  = *new Color(0x5e, 0x78, 0x45);
}
bool ChessWindow::isWhiteSquare(int idx) {
    return idx % 16 < 8 ? !(idx % 2) : idx % 2;
}
void ChessWindow::drawBoard(vector<int> highlightSquares) {
    RectangleShape square(Vector2f(
        size,
        size)
    );

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int val = i * 8 + j;
            int count = std::count(highlightSquares.begin(), highlightSquares.end(), val);
            if (isWhiteSquare(val)) {
                square.setFillColor(count ? whiteHighlight : whiteBackground);
            } else {
                square.setFillColor(count ? blackHighlight : blackBackground);
            }
            square.setPosition(
                size * j, 
                size * i);
            window->draw(square);
        }
    }
}
void ChessWindow::drawLabels() {
    Text label;
    label.setFont(labelFont);
    label.setCharacterSize(fontSize);

    for (int i = 0; i < 8; i++) {
        label.setString(std::to_string(8 - i));
        label.setPosition(Vector2f(
            labelLeftOffset, 
            size * i));
        label.setFillColor(i%2 ? whiteBackground : blackBackground);
        window->draw(label);
    }

    for (int i = 0; i < 8; i++) {
        label.setString(std::string(1, (char)'a' + i));
        label.setPosition(Vector2f(
            size * (i + 1) - labelRightOffset,
            size * 8 - fontSize - labelBottomOffset));
        label.setFillColor(i%2 ? blackBackground : whiteBackground);
        window->draw(label);
    }
}
void ChessWindow::drawMoveIndicators(vector<int> moveIndicatorSquares) {
    for (size_t i = 0; i < moveIndicatorSquares.size(); i++) {
        CircleShape circle(indicatorRad);
        circle.setFillColor(isWhiteSquare(moveIndicatorSquares[i]) ? whiteIndicator : blackIndicator);
        int extra = (size - indicatorRad*2) / 2;
        circle.setPosition(
            size * (moveIndicatorSquares[i] % 8) + extra,
            size * (moveIndicatorSquares[i] / 8) + extra);
        window->draw(circle);
    }
}
void ChessWindow::drawPieces(bool isDrag, int primaryIdx, Vector2i pos) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int idx = i * 8 + j;
            if (game.moveCalculator.board.isOccupied(idx)) {
                Square piece = game.moveCalculator.board[idx];
                int p = game.moveCalculator.board.getIdx(piece);
                Sprite pieceSprite = *new Sprite(Board::isWhite(piece) ? wpieces[p] : bpieces[p]);
                if (isDrag && primaryIdx == idx) {
                    pieceSprite.setPosition(
                        limitToRange(pos.x - size / 2, -size / 5, len), 
                        limitToRange(pos.y - size / 2, -size / 5, len));
                } else {
                    pieceSprite.setPosition(size * j, size * i);
                }
                window->draw(pieceSprite);
            }
        }
    }
}
int ChessWindow::limitToRange(int val, int min, int max) {
    return std::min(max, std::max(val, min) + size/2) - size/2;
}
int ChessWindow::findIdxByPos(Vector2i pos, int width, int height) {
    if (pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height)
        return -1;
    return pos.y * 8 / height * 8 + pos.x * 8 / width;
}