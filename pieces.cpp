#include "pieces.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
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
    isPin = false;
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
        if (position[V][H] == pieces[i])
            return true;
    }
    return false;
}

bool Piece::pmove(const int H, const int V, char (&position)[8][8], const int move){
    if (hereMyFigure(H, V, position))
        return false;

    if ( isPosible(H, V, position, move)){
        position[coordV][coordH] = ' ';
        coordH = H;
        coordV = V;
        position[coordV][coordH] = name;
        return true;
    }
    return false;
}

////

EnPassantMe Pawn::enpas = {0, 0, 0};

Pawn::Pawn(const int coordH, const int coordV, char name)
    : Piece(coordH, coordV, name){

    direc = col ? -1 : 1;
    isFirstMove = true;
}

void Pawn::setEnPasant(int H, int V, int m){
    enpas.coordH = H;
    enpas.coordV = V;
    enpas.move = m;
}

bool Pawn::isEnPasant(int H, int V, int m){

    return enpas.move == m  && enpas.coordH == H && enpas.coordV == V;
}


bool Pawn::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    if(coordH == H && (coordV == V - 1 * direc) || (coordV == V - 1 * direc && position[V][H] != ' ')){
        isFirstMove = false;
        return true;
    }
    else if (isFirstMove && coordV == V - 2 * direc){
        isFirstMove = false;
        setEnPasant(H, V, move);
        return true;
    }
    else if (isEnPasant(H, V - 1 * direc, move)) { // atack
        isFirstMove = false;
        position[enpas.coordV][enpas.coordH] = ' ';
        return true;
    } 
    cout << enpas.coordV << endl;
    cout << enpas.coordH << endl;
    cout << "blaha" << endl;
    return false;
}

bool Pawn::ishaveMove(char (&position)[8][8]){
    if ((isFirstMove && position[coordV + 2][coordH] == ' ') || position[coordV + 1][coordH] == ' '){
        return true;
    }
    if (position[coordV + 1][coordH + 1] != ' ' && !hereMyFigure( coordH + 1, coordV + 1, position) || 
        (position[coordV + 1][coordH - 1] != ' ' && !hereMyFigure( coordH - 1, coordV + 1, position))){
    
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
    
    if (coordH == H) {
        int start = std::min(coordV, V) + 1;
        int end = std::max(coordV, V);
        for (int i = start; i < end; ++i) {
            if (position[i][H] != ' ') {
                return false; 
            }
        }
        return true; 
    }
    if (coordV == V) {
        int start = std::min(coordH, H) + 1;
        int end = std::max(coordH, H);
        for (int i = start; i < end; ++i) {
            if (position[V][i] != ' ') {
                return false; 
            }
        }
        return true; 
    }
    return false;
}

bool Rock::ishaveMove(char (&position)[8][8]){
    for (int i = coordV + 1; i <= 7; i++){
        if (position[i][coordH] == ' ' || ! hereMyFigure(coordH, i, position)) return true;
        break;
    }
    for (int i = coordV + 1; i <= 0; i--){
        if (position[i][coordH] == ' ' || !hereMyFigure(coordH, i, position)) return true;
        break;
    }
    for (int i = coordH + 1; i <= 7; i--){
        if (position[coordV][i] == ' ' || !hereMyFigure(i, coordV, position)) return true;
        break;
    }
    for (int i = coordH + 1; i <= 0; i--){
        if (position[coordV][i] == ' ' || !hereMyFigure(i, coordV, position)) return true;
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
            if(position[V + i * moveV ][H + i * moveH] != ' ')
                return false;
            }
        return true;
    }
    return false;
}

bool Bishop::ishaveMove(char (&position)[8][8]){
    for (int i = coordH + 1, j = coordV + 1 ; i <= 7 && j <= 7; i++, j++){
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH + 1, j = coordV - 1 ; i <= 7 && j <= 0; i++, j--){
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH - 1, j = coordV + 1 ; i <= 0 && j <= 7; i--, j++){
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) return true;
        break;
    }
    for (int i = coordH + 1, j = coordV + 1 ; i <= 0 && j <= 0; i--, j--){
        if (position[j][i] == ' ' || !hereMyFigure(i, j, position)) return true;
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
    if (rock.isPosible(H, V, position, move) || bishop.isPosible(H, V, position, move)){
        bishop.coordV = V;
        bishop.coordH = H;
        rock.coordV = V;
        rock.coordH = H;
        return true;
    }
    return false;
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
            if (position[newV][newH] == ' ' || !hereMyFigure(newH, newV, position)) return true;
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
