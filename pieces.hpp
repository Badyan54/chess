#ifndef PIECES_HPP
#define PIECES_HPP

enum Color {BLACK, WHITE};

struct EnPassantMe{
    int coordH;
    int coordV;
    int move;
    
};

class Piece{
public:
    char name;
    Color col;
    int coordH;
    int coordV;

    Piece(const int H, const int V, const char n);
    bool hereMyFigure(int H, int V, char (&position)[8][8]);
    bool pmove(const int H, const int V, char (&position)[8][8], const int move);

    int get_coordH();
    int get_coordV();
    virtual bool ishaveMove(char (&position)[8][8]) = 0;
    virtual bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) = 0;
    
};

class Pawn : public Piece{
public:
    bool isFirstMove;
    static EnPassantMe enpas;
    int direc;
    Pawn(const int coordH, const int coordV, const char name);

    static void setEnPasant(int H, int V, int move);
    static bool isEnPasant(int H, int V, int move);
    virtual bool ishaveMove(char (&position)[8][8]) override;
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;
    void promote(const char name);

private:
    // Color col;

};

class Rock : public Piece{
public:
    bool isFirstMove;
    Rock(const int H, const int V, const char name);
    bool ishaveMove(char (&position)[8][8]) override;
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;

};

class Bishop : public Piece{
public:
    Bishop(const int H, const int V, const char name);
    bool ishaveMove(char (&position)[8][8]) override;
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;
};

class Queen : public Piece{
private:
Bishop bishop;
Rock rock; 
public:
    bool ishaveMove(char (&position)[8][8]) override;
    Queen(const int H, const int V, const char name);
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;
};

class King : public Piece{
public:
    bool isFirstMove;
    bool canCastle;
    bool ishaveMove(char (&position)[8][8]) override;
    King(const int H, const int V, const char name);
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;

};

class Knight : public Piece{
public:
    bool ishaveMove(char (&position)[8][8]) override;
    Knight(const int H, const int V, const char name);
    bool isPosible(const int coordH, const int coordV, char (&position)[8][8], const int move) override;
};

#endif // PIECES_HPP
