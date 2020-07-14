#include <SFML/Graphics.hpp>
#include <iostream>
#include "board/board.hpp"

using namespace sf;

constexpr int len = 600;
constexpr int size = len / 8;
constexpr int indicatorRad = size / 5;

Texture wpieces[6]{}, bpieces[6]{};
Sprite pieces[64]{};

bool isWhiteSquare(int idx) {
    return idx % 16 < 8 ? !(idx % 2) : idx % 2;
}
void drawBoard(RenderWindow& window, int highlightSquare) {
    RectangleShape square(Vector2f(75, 75));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int val = i * 8 + j;
            if (isWhiteSquare(val)) {
                if (val == highlightSquare) { // white
                    square.setFillColor(Color(0xf6, 0xf6, 0x82));
                } else {
                    square.setFillColor(Color(0xee, 0xee, 0xd2));
                }
            } else {
                if (val == highlightSquare) { // black
                    square.setFillColor(Color(0xba, 0xca, 0x44));
                } else {
                    square.setFillColor(Color(0x77, 0x96, 0x56));
                }
            }
            square.setPosition(size * j, size * i);
            window.draw(square);
        }
    }
}
void addMoveIndicator(RenderWindow& window, int indicatorSquare) {
    CircleShape circle(indicatorRad);

    if (isWhiteSquare(indicatorSquare)) {
        circle.setFillColor(Color(0xff, 0xff, 0xff));
    } else {
        circle.setFillColor(Color(0xff, 0xff, 0xff));
    }
    int extra = (size - indicatorRad*2) / 2;
    std::cout << extra << std::endl;
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
                pieces[k].setScale(Vector2f(0.6, 0.6));
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
    Board board = *new Board();
    board.loadPosition(std::string(board.INITIAL_POSITION), true);

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
    float dx = 0, dy = 0;
    bool isMove = false;

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
                    if (idx != -1 && board.isOccupied(idx)) {
                        if (pieces[idx].getGlobalBounds().contains(pos.x, pos.y)) {
                            isMove = true;
                            // calc diffs
                            dx = pos.x - pieces[idx].getPosition().x;
                            dy = pos.y - pieces[idx].getPosition().y;
                        }
                    }
                }
            }
            // drop event
            if (e.type == Event::MouseButtonReleased) {
                if (e.mouseButton.button == Mouse::Left) {
                    isMove = false;
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
        // add any move indicators above
        addMoveIndicator(window, 30);
        // assign the sprites array pieces
        setPieces(window, board);
        // set new position while dragging, check bounds
        if (isMove)
            pieces[idx].setPosition(
                limitToRange(pos.x - dx, -25, len), 
                limitToRange(pos.y - dy, -25, len));
        // draw all pieces from board
        for (int i = 0; i < 64; i++)
            if (board.isOccupied(i) && !(isMove && idx == i))
                window.draw(pieces[i]);
        // draw dragged piece so it is on top
        if (isMove)
            window.draw(pieces[idx]);
        window.display();
    }

    return 0;
}