#include <vector>
#include "pieces.hpp"

using namespace std;

enum Stun {PLAY, DRAW, WINWHITE, WINBLACK};

class Board{
private:
    std::vector<std::unique_ptr<Piece>> piecesWhite;
    std::vector<std::unique_ptr<Piece>> piecesBlack;
    King* kingBlack;
    King* kingWhite;
    const static char defaultPosition[8][8];
    char** position;
    int moveNum;
    
    char** initPos(char** position);
    char** initPos(char (&pos)[8][8]);
    void push_back_piece(char name, int i, int j);
    bool isUnderAttack(int X, int Y, Color col, char** position);
    Rock* get_rock(int X, Color col);
    King& getKing(Color col);
    bool set_promote();
    bool canCastle(King &king, Rock &rook, Color col);
    bool castle(King &king, Rock *rook, Color col);
    bool kingIsHaveMove(King king);
    void endGame();

public:
    Board();
    Board(const char (&myPosition)[8][8]);
    ~Board();

    void delPos(char** pos);
    char** get_position();
    void writeCurrentBoard();
    void startGame();
    bool isCheckMate(Color col);
    bool draw(Color col);
    bool move(int currentX, int currentY, int newX, int newY, Color col, char newName);
    Stun check_position(Color col);
};