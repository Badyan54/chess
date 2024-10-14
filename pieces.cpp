#include "pieces.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <memory>
using namespace std;

Piece::Piece(const int X, const int Y, const char n){
    if (n >= 'a' && n <= 'z') {
        col = WHITE;
    } else if (n >= 'A' && n <= 'Z') {
        col = BLACK;
    } 
    x = X;
    y = Y;
    name = n;
}

int Piece::get_x(){
    return x;
}
int Piece::get_y(){
    return y;
}

void Piece::set_coord(int X, int Y){
    x = X;
    y = Y;
}

bool Piece::hereMyFigure(int X, int Y, char (&position)[8][8]){
    string pieces = (col == WHITE) ? "prbhq": "PRBHQ";

    for (int i = 0; i < pieces.length(); i++){
        if (position[Y][X] == pieces[i])
            return true;
    }
    return false;
}

bool Piece::move(const int X, const int Y, char (&position)[8][8], const int move){
    if (hereMyFigure(X, Y, position))
        return false;

    if ( isPosible(X, Y, position, move)){
        position[y][x] = ' ';
        x = X;
        y = Y;
        position[y][x] = name;
        return true;
    }
    return false;
}

////

EnPassantMe Pawn::enpas = {0, 0, 0};

Pawn::Pawn(const int x, const int y, char name)
    : Piece(x, y, name){

    direc = col ? -1 : 1;
    isFirstMove = true;
}

void Pawn::setEnPasant(int X, int Y, int m){
    enpas.x = X;
    enpas.y = Y;
    enpas.move = m;
}

bool Pawn::isEnPasant(int X, int Y, int m){

    return enpas.move == m  && enpas.x == X && enpas.y == Y;
}


bool Pawn::isPosible(const int X, const int Y, char (&position)[8][8], const int move) {
    if (x == X) {
        if (y == Y - 1 * direc && position[Y][X] == ' ') {
            isFirstMove = false;
            return true;
        }
        if (isFirstMove && y == Y - 2 * direc) {
            isFirstMove = false;
            setEnPasant(X, Y, move);
            return true;
        }
    }
    // Перевірка на атаку "en passant"
    if (isEnPasant(X, Y - 1 * direc, move)) {
        isFirstMove = false;
        position[enpas.y][enpas.x] = ' ';
        return true;
    }
    return false;
}

bool Pawn::ishaveMove(char (&position)[8][8]) {
    if (isFirstMove && y + 2 < 8 && position[y + 2][x] == ' ') {
        return true; 
    }

    if (y + 1 < 8 && position[y + 1][x] == ' ') {
        return true; 
    }

    if (x + 1 < 8 && y + 1 < 8 && position[y + 1][x + 1] != ' ' && 
        !hereMyFigure(x + 1, y + 1, position)) {
        return true; 
    }
    if (x - 1 >= 0 && y + 1 < 8 && position[y + 1][x - 1] != ' ' && 
        !hereMyFigure(x - 1, y + 1, position)) {
        return true; 
    }

    return false;
}


Rock::Rock(const int x, const int y, const char name)
    : Piece(x, y,  name){
        hasMoved = false;
}

bool Rock::isPosible(const int X, const int Y, char (&position)[8][8], const int move){
    
    if (x == X) {
        int start = std::min(y, Y) + 1;
        int end = std::max(y, Y);
        for (int i = start; i < end; ++i) {
            if (position[i][X] != ' ') {
                return false; 
            }
        }
        hasMoved = true;
        return true; 
    }
    if (y == Y) {
        int start = std::min(x, X) + 1;
        int end = std::max(x, X);
        for (int i = start; i < end; ++i) {
            if (position[Y][i] != ' ') {
                return false; 
            }
        }
        hasMoved = true;
        return true; 
    }
    return false;
}

bool Rock::ishaveMove(char (&position)[8][8]) {
    // Перевірка руху вниз
    for (int i = y + 1; i <= 7; i++) {
        if (position[i][x] == ' ' || !hereMyFigure(x, i, position)) {
            return true;
        }
        if (!hereMyFigure(x, i, position)) { // Якщо фігура противника
            return true; // Можливий удар
        }
        break; // Якщо зустріли свою фігуру
    }

    // Перевірка руху вгору
    for (int i = y - 1; i >= 0; i--) {
        if (position[i][x] == ' ' || !hereMyFigure(x, i, position)) {
            
            return true;
        }
        if (!hereMyFigure(x, i, position)) {

            return true;
        }
        break; 
    }

    // Перевірка руху вправо
    for (int i = x + 1; i <= 7; i++) {
        if (position[y][i] == ' ' || !hereMyFigure(i, y, position)) {
            return true;
        }
        if (!hereMyFigure(i, y, position)) {
            return true;
        }
        break; 
    }

    // Перевірка руху вліво
    for (int i = x - 1; i >= 0; i--) {
        if (position[y][i] == ' ' || !hereMyFigure(i, y, position)) {
            return true;
        }
        if (!hereMyFigure(i, y, position)) {
            return true;
        }
        break; 
    }

    return false; // Якщо немає можливих ходів
}
bool Rock::isHasMoved(){
    return hasMoved;
}

Bishop::Bishop(const int x, const int y, const char name)
    : Piece(x, y, name){
}

bool Bishop::isPosible(const int X, const int Y, char (&position)[8][8], const int move) {
    int deltaX = abs(x - X);
    int deltaY = abs(y - Y);

    // Перевірка, чи слон рухається по діагоналі
    if (deltaX == deltaY) {
        int stepX = (X > x) ? 1 : -1;  // Напрямок по горизонталі
        int stepY = (Y > y) ? 1 : -1;  // Напрямок по вертикалі

        // Перевірка всіх клітин на шляху
        for (int i = 1; i < deltaX; i++) {
            if (position[y + i * stepY][x + i * stepX] != ' ') {
                return false;  // Якщо є фігура на шляху
            }
        }
        return true;  // Якщо шлях вільний
    }
    return false;  // Якщо не діагональний хід
}


bool Bishop::ishaveMove(char (&position)[8][8]) {
    for (int i = x + 1, j = y + 1; i <= 7 && j <= 7; i++, j++) {
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) {
            return true; 
        }
        if (hereMyFigure(i, j, position)) {
            break; 
        }
    }

    for (int i = x + 1, j = y - 1; i <= 7 && j >= 0; i++, j--) {
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) {
            return true; 
        }
        if (hereMyFigure(i, j, position)) {
            break; 
        }
    }

    for (int i = x - 1, j = y + 1; i >= 0 && j <= 7; i--, j++) {
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) {
            return true; 
        }
        if (hereMyFigure(i, j, position)) {
            break; 
        }
    }

    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) {
            return true; 
        }
        if (hereMyFigure(i, j, position)) {
            break; 
        }
    }

    return false; 
}


Queen::Queen(const int x, const int y, const char name)
    : Piece(x, y, name),
    rock(x, y, name),
    bishop(x, y, name){
}

bool Queen::isPosible(const int X, const int Y, char (&position)[8][8], const int move){
    if (rock.isPosible(X, Y, position, move) || bishop.isPosible(X, Y, position, move)){
        bishop.set_coord(X, Y);
        rock.set_coord(X, Y);
        return true;
    }
    return false;
}

bool Queen::ishaveMove(char (&position)[8][8]){
    return (rock.ishaveMove(position) || bishop.ishaveMove(position));
}

Knight::Knight(const int x, const int y, const char name)
    : Piece(x, y, name){

}
bool Knight::isPosible(const int X, const int Y, char (&position)[8][8], const int move){
    int deltaX = abs(x - X);
    int deltaY = abs(y - Y);

    return (deltaX == 1 && deltaY == 2) || (deltaX == 2 && deltaY == 1);

}

bool Knight::ishaveMove(char (&position)[8][8]) {
    int moves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (int i = 0; i < 8; ++i) {
        int newX = x + moves[i][0]; 
        int newY = y + moves[i][1];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (position[newY][newX] == ' ' || !hereMyFigure(newX, newY, position)) {
                return true; 
            }
        }
    }
    return false; 
}

King::King(const int x, const int y, const char name)
    : Piece(x, y, name){
    hasMoved = false;
}

bool King::isPosible(const int X, const int Y, char (&position)[8][8], const int move){
    int deltaX = abs(x - X);
    int deltaY = abs(y - Y);

    if (deltaX <= 1 && deltaY <= 1){
        hasMoved = true;
        return true;
    }
    return false;
    
}

bool King::ishaveMove(char (&position)[8][8]) {
    int moves[8][2] = {
        {1, 0},  {1, 1},  {0, 1},  {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };

    for (int i = 0; i < 8; ++i) {
        int newX = x + moves[i][0];
        int newY = y + moves[i][1];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if (position[newY][newX] == ' ' || !hereMyFigure(newX, newY, position)) {
                return true; 
            }
        }
    }
    return false; 
}

bool King::isHasMoved(){
    return hasMoved;
}


