#ifndef PIECES_HPP
#define PIECES_HPP
#include <vector>
#include <memory>
enum Color { WHITE, BLACK};

struct EnPassantMe{
    int x;
    int y;
    int move;
    
};

class Piece{
protected:
    int x;
    int y;
public:
    bool hereMyFigure(int X, int Y, char** position);
    char name;
    Color col;
    Piece(const int X, const int Y, const char n);
    bool move(const int X, const int Y, char** position, const int move);
    int get_x();
    int get_y();
    void set_coord(int X, int Y);

    virtual bool ishaveMove(char** position) = 0;
    virtual bool isPosible(const int x, const int y, char** position, const int move) = 0;
    
};

class Pawn : public Piece{
private:
    bool isFirstMove;
    int direc;
    static EnPassantMe enpas;
    static void setEnPasant(int X, int Y, int move);
    static bool isEnPasant(int X, int Y, int move);
public:
    Pawn(const int x, const int y, const char name);
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;
};

class Rock : public Piece{
private:
    bool hasMoved;
public:
    Rock(const int X, const int Y, const char name);
    bool isHasMoved();
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;

};

class Bishop : public Piece{
public:
    Bishop(const int X, const int Y, const char name);
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;
};

class Queen : public Piece{
private:
    Bishop bishop;
    Rock rock; 
public:
    Queen(const int X, const int Y, const char name);
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;
};

class King : public Piece{
private:
    bool hasMoved;
public:
    King(const int X, const int Y, const char name);
    bool isHasMoved();
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;
   // bool isUnderAttack(int X, int Y, std::vector<std::unique_ptr<Piece>>& opponentPieces, char** position);
};

class Knight : public Piece{
public:
    Knight(const int X, const int Y, const char name);
    bool ishaveMove(char** position) override;
    bool isPosible(const int x, const int y, char** position, const int move) override;
};

#endif // PIECES_HPP
