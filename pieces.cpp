#include "pieces.hpp"
#include <algorithm>
#include <string>

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

bool Piece::move(const int H, const int V, char (&position)[8][8], const int move){
    string pieces = (col == WHITE) ? "PRBHQ" : "prbhq";


    for (int i = 0; i < pieces.length(); i++){
        if (position[H][V] == pieces[i])
            return false;
    }

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

// void Pawn::promote(const char name, ){
//     Queen queen ;
// }

/////

Rock::Rock(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV,  name){
}

bool Rock::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    if (coordH == H){
        int i = i < V ? V + 1 : V - 1;
        while (i != coordV ){
            if (position[H][i] != ' ')
                return false;
            i < V ? i++ : i--;
        }
        return true;
    }
    else if (coordV == V){
        int i = i < H ? H + 1 : H - 1;
        while( i != coordH ){
            if (position[i][V] != ' ')
                return false;
            i < H ? i++ : i--;
        }
        return true;
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

Queen::Queen(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name),
    rock(coordH, coordV, name),
    bishop(coordH, coordV, name){
}

bool Queen::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    return (rock.isPosible(H, V, position, move) || bishop.isPosible(H, V, position, move));
}

King::King(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name){
}

bool King::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    int deltaH = abs(coordH - H);
    int deltaV = abs(coordV - V);

    return deltaH <= 1 && deltaV <= 1;
}

Knight::Knight(const int coordH, const int coordV, const char name)
    : Piece(coordH, coordV, name){

}
bool Knight::isPosible(const int H, const int V, char (&position)[8][8], const int move){
    int deltaH = abs(coordH - H);
    int deltaV = abs(coordV - V);

    return (deltaH == 1 && deltaV == 2) || (deltaH == 2 && deltaV == 1);

}
