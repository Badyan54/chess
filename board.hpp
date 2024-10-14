#include <memory>
#include <vector>
#include "pieces.hpp"

enum Stun {PLAY, DRAW, WINWHITE, WINBLACK};

class Board{
public:
    Board();
    Board(const char myPosition[8][8]);
    int numverOfMove;
    const static char defaultPosition[8][8];
    char position[8][8];
    std::vector<std::unique_ptr<Piece>> piecesWhite;
    std::vector<std::unique_ptr<Piece>> piecesBlack;
    King* kingBlack;
    King* kingWhite;


    void writeCurrentBoard();
    void push_back_piece(char name, int i, int j);
    void startGame();
    bool isCheckMate(Color col);
    bool draw(Color col);
    bool move(int currentCoordH, int currentCoordV, int newCoordH, int newCoordV, Color col, char newName);
    King& getKing(Color col);
    bool set_promote();
    Stun check_position(Color col);
    bool canCastle(King &king, Rock &rook, std::vector<std::unique_ptr<Piece>>& opponentPieces);
    bool castle(King &king, Rock *rook, std::vector<std::unique_ptr<Piece>>& opponentPieces);
    Rock* get_rock(int H, std::vector<std::unique_ptr<Piece>>& pieces, Color col);

};