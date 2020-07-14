#include <SFML/Graphics.hpp>
#include <iostream>
#include "board/board.hpp"

using namespace sf;

int len = 600;
int size = len / 8;

Texture wpieces[6]{}, bpieces[6]{};
Sprite pieces[64]{};

void drawBoard(RenderWindow& window) {
    Vector2f v(75, 75);
    RectangleShape whiteSquare(v);
    RectangleShape blackSquare(v);
    whiteSquare.setFillColor(Color(0xeb, 0xec, 0xd0));
    blackSquare.setFillColor(Color(0x77, 0x95, 0x56));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int val = i * 8 + j;
            if (val%16<8?val%2:val%2==0) {
                blackSquare.setPosition(75 * j, 75 * i);
                window.draw(blackSquare);
            } else {
                whiteSquare.setPosition(75 * j, 75 * i);
                window.draw(whiteSquare);
            }
        }
    }
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

    bool isMove = false;
    int idx = 0;
    float dx = 0, dy = 0;

    while (window.isOpen()){
        Vector2i pos = Mouse::getPosition(window);

        Event e;
        while (window.pollEvent(e)){
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    idx = findIdxByPos(pos);
                    if (idx != -1 && board.isOccupied(idx)) {
                        if (pieces[idx].getGlobalBounds().contains(pos.x, pos.y)) {
                            isMove = true;
                            dx = pos.x - pieces[idx].getPosition().x;
                            dy = pos.y - pieces[idx].getPosition().y;
                        }
                    }
                }
            }
            if (e.type == Event::MouseButtonReleased) {
                if (e.mouseButton.button == Mouse::Left) {
                    isMove = false;
                    int newIdx = findIdxByPos(pos);
                    if (newIdx != -1 && !board.isOccupied(newIdx)) {
                        board.squares[newIdx] = board.squares[idx];
                        board.squares[idx] = Empty;
                    }
                }
            }
        }

        window.clear();
        drawBoard(window);
        setPieces(window, board);
        if (isMove)
            pieces[idx].setPosition(
                limitToRange(pos.x - dx, -25, len), 
                limitToRange(pos.y - dy, -25, len));
        for (int i = 0; i < 64; i++)
            if (board.isOccupied(i) && !(isMove && idx == i))
                window.draw(pieces[i]);
        if (isMove)
            window.draw(pieces[idx]);
        window.display();
    }

    return 0;
}