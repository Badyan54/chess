#ifndef BOARD_HPP
#define BOARD_HPP
#include <vector>
#include <memory>
#include "pieces.hpp"


class Piece;

class Board{
private:
    
    static const char defaultPosition[8][8];
    char position[8][8];

public:
    Board();
    Board(const char myPosition[8][8]);
    void startGame(std::vector<std::unique_ptr<Piece>>& piecesWhite,
                    std::vector<std::unique_ptr<Piece>>& piecesBlack);
    bool isCheck(int coordH, int coordV, std::vector<std::unique_ptr<Piece>>& pieces, char (&testPosition1)[8][8]);
    bool isCheckMate(std::vector<std::unique_ptr<Piece>>& pieces1,
                    std::vector<std::unique_ptr<Piece>>& pieces2, King &King);
    void writeCurrentBoard();
    bool draw(std::vector<std::unique_ptr<Piece>>& pieces, King &king);
};

#endif 