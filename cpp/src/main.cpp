#include <SFML/Graphics.hpp>
#include <iostream>
#include "board/board.hpp"

using namespace sf;

constexpr int len = 1000;
constexpr int size = len / 8;
constexpr int indicatorRad = size / 5;
constexpr int fontSize = size / 4;
constexpr int labelLeftOffset = size / 15;

Font labelFont;
Texture wpieces[6]{}, bpieces[6]{};
Sprite pieces[64]{};
Color  whiteHighlight(0xf6, 0xf6, 0x82);
Color whiteBackground(0xee, 0xee, 0xd2);
Color  blackHighlight(0xba, 0xca, 0x44);
Color blackBackground(0x77, 0x96, 0x56);
Color  whiteIndicator(0xbe, 0xbe, 0xa8);
Color  blackIndicator(0x5e, 0x78, 0x45);

bool isWhiteSquare(int idx) {
    return idx % 16 < 8 ? !(idx % 2) : idx % 2;
}
void drawBoard(RenderWindow& window, int highlightSquare) {
    RectangleShape square(Vector2f(size, size));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int val = i * 8 + j;
            if (isWhiteSquare(val)) {
                square.setFillColor(val == highlightSquare ? whiteHighlight : whiteBackground);
            } else {
                square.setFillColor(val == highlightSquare ? blackHighlight : blackBackground);
            }
            square.setPosition(size * j, size * i);
            window.draw(square);
        }
    }
}
void drawLabels(RenderWindow& window) {
    Text label;
    label.setFont(labelFont);
    label.setCharacterSize(fontSize);

    for (int i = 0; i < 8; i++) {
        label.setString(std::to_string(8 - i));
        label.setPosition(Vector2f(
            labelLeftOffset, 
            size * i));
        label.setColor(i%2 ? whiteBackground : blackBackground);
        window.draw(label);
    }

    for (int i = 0; i < 8; i++) {
        std::string((char)('a' + i));
        label.setString();
        label.setPosition(Vector2f(
            size * (i + 1) - 2 * labelLeftOffset,
            size * 8 - fontSize));
        label.setColor(i%2 ? whiteBackground : blackBackground);
        window.draw(label);
    }
}
void addMoveIndicator(RenderWindow& window, int indicatorSquare) {
    CircleShape circle(indicatorRad);
    circle.setFillColor(isWhiteSquare(indicatorSquare) ? whiteIndicator : blackIndicator);
    int extra = (size - indicatorRad*2) / 2;
    circle.setPosition(
        size * (indicatorSquare%8) + extra,
        size * (indicatorSquare/8) + extra);
    window.draw(circle);
}
void setPieces(RenderWindow& window, Board& board) {
    int k = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int idx = i * 8 + j;
            if (board.isOccupied(idx)) {
                Square piece = board.squares[idx];
                int p = board.getIdx(piece);
                pieces[k] = *new Sprite(board.isWhite(piece) ? wpieces[p] : bpieces[p]);
                pieces[k].setPosition(size * j, size * i);
                pieces[k].setScale(Vector2f(1, 1));
            }
            k++;
        }
    }
}
int limitToRange(int val, int min, int max) {
    return std::min(max, std::max(val, min) + size/2) - size/2;
}
int findIdxByPos(Vector2i pos) {
    if (pos.x < 0 || pos.x > len || pos.y < 0 || pos.y > len)
        return -1;
    return (pos.y / size) * 8 + pos.x / size;
}

int main() {
    RenderWindow window(VideoMode(len, len), "Chess");
    
    // set up board
    Board board;
    board.loadPosition(std::string(board.INITIAL_POSITION), true);
    
    // load font
    labelFont.loadFromFile("assets/fonts/ubuntu.ttf");

    // load piece graphics
    wpieces[board.getIdx(WPawn)  ].loadFromFile("assets/pieces/w/pawn.png");
    wpieces[board.getIdx(WKnight)].loadFromFile("assets/pieces/w/knight.png");
    wpieces[board.getIdx(WBishop)].loadFromFile("assets/pieces/w/bishop.png");
    wpieces[board.getIdx(WRook)  ].loadFromFile("assets/pieces/w/rook.png");
    wpieces[board.getIdx(WQueen) ].loadFromFile("assets/pieces/w/queen.png");
    wpieces[board.getIdx(WKing)  ].loadFromFile("assets/pieces/w/king.png");

    bpieces[board.getIdx(BPawn)  ].loadFromFile("assets/pieces/b/pawn.png");
    bpieces[board.getIdx(BKnight)].loadFromFile("assets/pieces/b/knight.png");
    bpieces[board.getIdx(BBishop)].loadFromFile("assets/pieces/b/bishop.png");
    bpieces[board.getIdx(BRook)  ].loadFromFile("assets/pieces/b/rook.png");
    bpieces[board.getIdx(BQueen) ].loadFromFile("assets/pieces/b/queen.png");
    bpieces[board.getIdx(BKing)  ].loadFromFile("assets/pieces/b/king.png");

    int idx = -1;
    bool isDrag = false;
    bool primarySelected = false;

    while (window.isOpen()){
        Vector2i pos = Mouse::getPosition(window);

        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed)
                window.close();

            // start drag
            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    idx = findIdxByPos(pos); // find square
                    if (idx != -1 && board.isOccupied(idx))
                        if (pieces[idx].getGlobalBounds().contains(pos.x, pos.y))
                            isDrag = true;
                }
            }
            // drop event
            if (e.type == Event::MouseButtonReleased) {
                if (e.mouseButton.button == Mouse::Left) {
                    isDrag = false;
                    int newIdx = findIdxByPos(pos);
                    // validate and execute the move
                    if (newIdx != -1 && !board.isOccupied(newIdx)) {
                        board.squares[newIdx] = board.squares[idx];
                        board.squares[idx] = Empty;
                    }
                }
            }
        }

        window.clear();
        // draw the board tiles
        drawBoard(window, board.isOccupied(idx) ? idx : -1);
        drawLabels(window);
        // add any move indicators above
        // addMoveIndicator(window, 30);
        // assign the sprites array pieces
        setPieces(window, board);
        // set new position while dragging, check bounds
        if (isDrag)
            pieces[idx].setPosition(
                limitToRange(pos.x - size / 2, -25, len), 
                limitToRange(pos.y - size / 2, -25, len));
        // draw all pieces from board
        for (int i = 0; i < 64; i++)
            if (board.isOccupied(i) && !(isDrag && idx == i))
                window.draw(pieces[i]);
        // draw dragged piece last so it is on top
        if (isDrag)
            window.draw(pieces[idx]);
        window.display();
    }

    return 0;
}