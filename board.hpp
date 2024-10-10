#include <vector>
#include <pieces.hpp>
enum Stun {PLAY, DRAW, WINWHITE, WINBLACK};

class Board{

    Board();
    Board(const char myPosition[8][8]);

    std::vector<std::unique_ptr<Piece>> piecesWhite;
    std::vector<std::unique_ptr<Piece>> piecesBlack;

    const static char defaultPosition[8][8];
    void writeCurrentBoard();
    void push_back_piece(char name, int i, int j);
    void startGame();
    bool move(int currentCoordH, int currentcoordV, int newcoordH, int newCoordV, char newName, Color col);
    bool isCheckMate(Color col);
    bool draw(Color col);
    King& getKing(std::vector<std::unique_ptr<Piece>>& pieces);
    bool set_promote();
    Stun check_position(Color col);

}