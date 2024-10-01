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
    int numverOfMove;

public:
    Board();
    Board(const char myPosition[8][8]);
    void startGame(std::vector<std::unique_ptr<Piece>>& piecesWhite,
                    std::vector<std::unique_ptr<Piece>>& piecesBlack);
    bool isCheckMate(std::vector<std::unique_ptr<Piece>>& pieces1,
                    std::vector<std::unique_ptr<Piece>>& pieces2, King &King);

    bool isCheck(int coordH, int coordV, std::vector<std::unique_ptr<Piece>>& pieces, char (&testPosition1)[8][8]);
    bool draw(std::vector<std::unique_ptr<Piece>>& pieces, King &king);

    bool move(int currentCoordH, int currentcoordV, int newcoordH, int newCoordV, King King,
                    std::vector<std::unique_ptr<Piece>>& pieces, std::vector<std::unique_ptr<Piece>>& pieces2, char newName );
    void writeCurrentBoard();
    void push_back_piece(char name, int i, int j,
            std::vector<std::unique_ptr<Piece>>& piecesWhite,
            std::vector<std::unique_ptr<Piece>>& piecesBlack);
    void endGame(Color col, std::vector<std::unique_ptr<Piece>> piecesWhite,
    std::vector<std::unique_ptr<Piece>> piecesBlack);
    
};

#endif 