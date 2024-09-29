#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>

class Board{
private:

    static const char defaultPosition[8][8];
    char position[8][8];

public:
    Board();
    Board(const char myPosition[8][8]);
    void startGame();
    bool isChack();
    void writeCurrentBoard();
};

#endif 