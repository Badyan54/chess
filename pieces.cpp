#include "pieces.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
using namespace std;

Piece::Piece(const int H, const int V, const char n){
    if (n >= 'a' && n <= 'z') {
        col = BLACK;
    } else if (n >= 'A' && n <= 'Z') {
        col = WHITE;
    } 
    coordH = H;
    coordV = V;
    name = n;
}

int Piece::get_coordH(){
    return coordH;
}
int Piece::get_coordV(){
    return coordV;
}


bool Piece::hereMyFigure(int H, int V, char (&position)[8][8]){
    string pieces = (col == WHITE) ? "PRBHQ" : "prbhq";


    for (int i = 0; i < pieces.length(); i++){
        if (position[H][V] == pieces[i])
            return true;
    }
    return false;
}

bool Piece::move(const int H, const int V, char (&position)[8][8], const int move){
    if (hereMyFigure(H, V, position))
        return false;

    if ( isPosible(H, V, position, move)){
        position[coordH][coordV] = ' ';
        coordH = H;
        coordV = V;
        position[coordH][coordV] = name;
        return true;
    }
    return false;
}

////

EnPassantMe Pawn::enpas = {0, 0, 0};

Pawn::Pawn(const int coordH, const int coordV, char name)
    : Piece(coordH, coordV, name){

    direc = col ? 1 : -1;
    isFirstMove = true;
}

void Pawn::setEnPasant(int H, int V, int m){
    enpas.coordH = H;
    enpas.coordV = V;
    enpas.move = m;
}

bool Pawn::isEnPasant(int H, int V, int m){
    return enpas.move == m - 1 && enpas.coordH == H && enpas.coordV == V;
}


bool Pawn::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    if(coordH == H && (coordV == V - 1 * direc)){
        isFirstMove = false;
        return true;
    }
    else if (isFirstMove && coordV == V - 2 * direc){
        setEnPasant(coordH, coordV, move);
        return true;
    }
    else if ((coordV == V - 1 * direc && position[H][V] != ' ') || 
    (isEnPasant(coordH, V - 1 * direc, move))){ // atack
        isFirstMove = false;
        return true;
    } 
    return false;
}

bool Pawn::ishaveMove(char (&position)[8][8]){
    if ((isFirstMove && position[coordH][coordV + 2] == ' ') || position[coordH][coordV + 1] == ' '){
        return true;
    }
    if (position[coordH + 1][coordV + 1] != ' ' && !hereMyFigure( coordH + 1, coordV + 1, position) || 
        (position[coordH - 1][coordV + 1] != ' ' && !hereMyFigure( coordH - 1, coordV + 1, position))){
    
        return true;
    }
    return false;
}

// void Pawn::promote(const char name, ){
//     Queen queen ;
// }

/////

Rock::Rock(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV,  name){
}

bool Rock::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    if (coordH == H){
        int i = V;
        while (i != coordV ){
            if (position[H][i] != ' ')
                return false;
            i < V ? i++ : i--;
        }
        return true;
    }
    else if (coordV == V){
        int i = H;
        while( i != coordH ){
            if (position[i][V] != ' ')
                return false;
            i < H ? i++ : i--;
        }
        return true;
    }
    return false;
}

bool Rock::ishaveMove(char (&position)[8][8]){
    for (int i = coordV + 1; i <= 7; i++){
        if (position[coordH][i] == ' ' || ! hereMyFigure(coordH, i, position)) return true;
        break;
    }
    for (int i = coordV + 1; i <= 0; i--){
        if (position[coordH][i] == ' ' || !hereMyFigure(coordH, i, position)) return true;
        break;
    }
    for (int i = coordH + 1; i <= 7; i--){
        if (position[i][coordV] == ' ' || !hereMyFigure(i, coordV, position)) return true;
        break;
    }
    for (int i = coordH + 1; i <= 0; i--){
        if (position[i][coordV] == ' ' || !hereMyFigure(i, coordV, position)) return true;
        break;
    }
    return false;
}

Bishop::Bishop(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name){
}

bool Bishop::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    int deltaH = abs(coordH - H); int deltaV = abs(coordV - V);

    if ( deltaH == deltaV ){

        int moveH = coordH > H ? -1 : 1;
        int moveV = coordV > V ? -1 : 1;

        for(int i = 1; i < deltaH; i++ ){
            if(position[H + i * moveH][V + i * moveV ] != ' ')
                return false;
            }
        return true;
    }
    return false;
}

bool Bishop::ishaveMove(char (&position)[8][8]){
    for (int i = coordH + 1, j = coordV + 1 ; i <= 7 && j <= 7; i++, j++){
        if (position[i][j] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH + 1, j = coordV - 1 ; i <= 7 && j <= 0; i++, j--){
        if (position[i][j] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH - 1, j = coordV + 1 ; i <= 0 && j <= 7; i--, j++){
        if (position[i][j] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH + 1, j = coordV + 1 ; i <= 0 && j <= 0; i--, j--){
        if (position[i][j] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    return false;
}

Queen::Queen(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name),
    rock(coordH, coordV, name),
    bishop(coordH, coordV, name){
}

bool Queen::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    return (rock.isPosible(H, V, position, move) || bishop.isPosible(H, V, position, move));
}

bool Queen::ishaveMove(char (&position)[8][8]){
    return (rock.ishaveMove(position) || bishop.ishaveMove(position));
}



Knight::Knight(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name){

}
bool Knight::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    int deltaH = abs(coordH - H);
    int deltaV = abs(coordV - V);

    return (deltaH == 1 && deltaV == 2) || (deltaH == 2 && deltaV == 1);

}

bool Knight::ishaveMove(char (&position)[8][8]){
    int moves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    for (int i = 0; i < 8; ++i) {
        int newH = newH + moves[i][0];
        int newV = newV + moves[i][1];
        if (newH >= 0 && newH <= 7 && newV >= 0 && newV <= 7){
            if (position[newH][newV] == ' ' || !hereMyFigure(newH, newV, position)) return true;
        }
    }
    return false;
}


King::King(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name){
}

bool King::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    int deltaH = abs(coordH - H);
    int deltaV = abs(coordV - V);

    return deltaH <= 1 && deltaV <= 1;
}

bool King::ishaveMove(char (&position)[8][8]){
    return false;
}
