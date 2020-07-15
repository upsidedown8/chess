#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "game/game.hpp"

using namespace sf;
using namespace std;

constexpr int len = 1000;
constexpr int size = len / 8;
constexpr int indicatorRad = size / 5;
constexpr int fontSize = size / 4;
constexpr int labelLeftOffset = size / 15;
constexpr int labelRightOffset = size / 5;
constexpr int labelBottomOffset = size / 15;

class ChessWindow {
private:
    Color whiteHighlight, whiteBackground;
    Color blackHighlight, blackBackground;
    Color whiteIndicator, blackIndicator;

    Font labelFont;
    Texture wpieces[6]{}, bpieces[6]{};
    
    Game game;
    RenderWindow *window;

    void setup();
    bool isWhiteSquare(int idx);
    void drawBoard(vector<int> highlightSquares);
    void drawLabels();
    void drawMoveIndicators(vector<int> moveIndicatorSquares);
    void drawPieces(bool isDrag, int primaryIdx, Vector2i pos);
    int limitToRange(int val, int min, int max);
    int findIdxByPos(Vector2i pos, int width, int height);

public:
    ChessWindow();

    void start();
};