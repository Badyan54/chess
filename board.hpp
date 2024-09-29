#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include <memory>

class Piece;

class Board{
private:
    
    static const char defaultPosition[8][8];
    char position[8][8];

public:
    Board();
    Board(const char myPosition[8][8]);
    void startGame(std::vector<std::unique_ptr<Piece>>& pieces);
    bool isChack();
    void writeCurrentBoard();
};

#endif 