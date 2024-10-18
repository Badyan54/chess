#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

Board::Board(){
    position = new char*[8];
    for (int i = 0; i < 8; ++i) {
        position[i] = new char[8];
        for (int j = 0; j < 8; ++j) {
            position[i][j] = defaultPosition[i][j];
        }
    }
    moveNum = 0;
}
Board::~Board(){
    delPos(position);
}

void Board::delPos(char** pos){
    for (int i = 0; i < 8; ++i) {
        delete[] pos[i];
    }
    delete[] pos;
}

Board::Board(const char (&myPosition)[8][8]) {
    position = new char*[8];
    for (int i = 0; i < 8; ++i) {
        position[i] = new char[8];
        for (int j = 0; j < 8; ++j) {
            position[i][j] = myPosition[i][j];
        }
    }
    moveNum = 0;
}

char** Board::initPos(char** pos) {
    char** newPosition = new char*[8]; 
    for (int i = 0; i < 8; ++i) {
        newPosition[i] = new char[8]; 
        for (int j = 0; j < 8; ++j) {
            newPosition[i][j] = pos[i][j]; 
        }
    }
    return newPosition; 
}

const char Board::defaultPosition[8][8] = {
    {'r', ' ', ' ', 'q', 'k', ' ', ' ', 'r'},
    {'p', 'p', 'P', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', 'P', 'P', 'P', 'P', 'P', ' '},
    {'R', ' ', ' ', 'K', ' ', ' ', ' ', 'R'}
};

void Board::writeBoard() {
    cout << "    A B C D E F G H" << std::endl;
    for (int i = 0; i < 8; i++) {
        cout << i + 1 << "   ";
        for (int j = 0; j < 8; j++) {
            cout << position[i][j] << " ";
        }
        cout << std::endl;
    }
}

char** Board::get_position() {
    return position;
}

void Board::push_back_piece(char name, int i, int j){                
    switch(name){
        case 'P': 
            piecesBlack.push_back(make_unique<Pawn>(i, j, 'P'));
            break;

        case 'p':  
            piecesWhite.push_back(make_unique<Pawn>(i, j, 'p'));
            break;

        case 'R':
            piecesBlack.push_back(make_unique<Rock>(i, j, 'R'));
            break;

        case 'r':
        
            piecesWhite.push_back(make_unique<Rock>(i, j, 'r'));
            break;

        case 'H':
            piecesBlack.push_back(make_unique<Knight>(i, j, 'H'));
            break;

        case 'h':
            piecesWhite.push_back(make_unique<Knight>(i, j, 'h'));
            break;

        case 'B':
            piecesBlack.push_back(make_unique<Bishop>(i, j, 'B'));
            break;
            
        case 'b':
            piecesWhite.push_back(make_unique<Bishop>(i, j, 'b'));
            break;

        case 'Q':
            piecesBlack.push_back(make_unique<Queen>(i, j, 'Q'));
            break;
        
        case 'q':
            piecesWhite.push_back(make_unique<Queen>(i, j, 'q'));
            break;

        case 'K':
            piecesBlack.push_back(make_unique<King>(i, j, 'K'));
            break;

        case 'k':
            piecesWhite.push_back(make_unique<King>(i, j, 'k'));
            break;
    }
}

void Board::startGame(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            push_back_piece(position[j][i], i, j);
        }
    }
    kingWhite = &getKing(WHITE);
    kingBlack = &getKing(BLACK);
}

bool Board::isCheckMate(Color col) {
    King& king = col ? (*kingBlack) : (*kingWhite);
    int kingX = king.get_x();
    int kingY = king.get_y();
    if (!(isUnderAttack(kingX, kingY, col, position))) return false;

    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    char** testPosition = initPos(position);

    for (auto& piece : opponentPieces) {
        int attackerX = piece->get_x();
        int attackerY = piece->get_y();
        
        if (piece->isPosible(kingX, kingY, position, 2)) {
            
            int dX = (attackerX - kingX) / std::max(1, std::abs(attackerX - kingX));
            int dY = (attackerY - kingY) / std::max(1, std::abs(attackerY - kingY));
            int X = kingX + dX;
            int Y = kingY + dY;

            while (X != attackerX || Y != attackerY) {
                if (isUnderAttack(X, Y, col, position)) {
                    char before = testPosition[Y][X];
                    testPosition[Y][X] = ' '; 
                    if (isUnderAttack(kingX, kingY, col, testPosition)) {
                        delPos(testPosition);
                        return false;
                    }
                    testPosition[Y][X] = before;
                }
                X += dX;
                Y += dY;
            }
        }
    }
    delPos(testPosition);
    return true; 
}

bool Board::draw(Color col) {
    King& king = col ? (*kingBlack) : (*kingWhite);
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    if (isUnderAttack(king.get_x(), king.get_y(), col, position)) return false;

    for (auto& piece : myPieces) {
        if (piece->ishaveMove(position) || kingIsHaveMove(king)) {
            return false; 
        }
    }
    return true;
}

bool Board::move(int currentX, int currentY, int newX, int newY, Color col) {  
    King& king = col ? (*kingBlack) : (*kingWhite); 
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    char** testPosition = initPos(position);
    testPosition[currentY][currentX] = ' ';
    testPosition[newY][newX] = 'R';

    if (isUnderAttack(king.get_x(), king.get_y(), col, testPosition)){
        delPos(testPosition);
        return false;
    }
    
    
    for (auto piece = myPieces.begin(); piece != myPieces.end(); ++piece) {
        if ((*piece)->get_x() == currentX && (*piece)->get_y() == currentY) {
            //castle
            if (king.get_x() == currentX && king.get_y() == currentY){
                if(abs(king.get_x() - newX) == 2){
                    return castle(king, get_rock(newX, col), col);
                }
            }
            //promote
            if ((*piece)->move(newX, newY, position, 2)) {
                
                if (((*piece)->name == 'P' || (*piece)->name == 'p') && (newY == 7 || newY == 0)) {
                    char newPiece;
                    cout << "Promote your pawn! Choose Queen, Rook, Bishop, Knight: ";
                    cin >> newPiece;
                    piece = myPieces.erase(piece);

                    push_back_piece(newPiece, newX, newY);
                    position[newY][newX] = newPiece;
                } 

                for (auto it = opponentPieces.begin(); it != opponentPieces.end(); ) {
                    if ((*it)->get_x() == newX && (*it)->get_y() == newY) {
                        it = opponentPieces.erase(it);
                    } else {
                        ++it;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

Stun Board::check_position(Color col){
    if (isCheckMate(col)) return col ? WINBLACK : WINWHITE;
    if (draw(col)) return DRAW;
    return PLAY;
}

bool Board::canCastle(King &king, Rock &rook, Color col) {
    if (king.isHasMoved()|| rook.isHasMoved() || isUnderAttack(king.get_x(), king.get_y(), col, position)) return false;

    int startX = king.get_x() < rook.get_x() ? king.get_x() : rook.get_x();
    int endX = king.get_x() > rook.get_x() ? king.get_x() : rook.get_x();

    for (int i = startX + 1; i < endX; ++i) {
        if (position[king.get_y()][i] != ' '|| isUnderAttack( i, king.get_y(), col, position)) return false;
        
    }
    return true; 
}

bool Board::castle(King &king, Rock *rook, Color col) {
    if (rook == nullptr) return false;

    if (canCastle(king, (*rook), col)) {
        if ((*rook).get_x() > king.get_x()) {
            position[king.get_y()][king.get_x() + 1] = (*rook).name;
            position[king.get_y()][king.get_x() + 2] = king.name;
            position[rook->get_y()][rook->get_x()] = position[king.get_y()][king.get_x()] = ' ';
            (*rook).set_coord(king.get_x() + 1, king.get_y());
            king.set_coord(king.get_x() + 2, king.get_y());

        } else { 
            position[king.get_y()][king.get_x() - 1] = (*rook).name;
            position[king.get_y()][king.get_x() - 2] = king.name;
            position[rook->get_y()][rook->get_x()] = position[king.get_y()][king.get_x()] = ' ';
            (*rook).set_coord(king.get_x() - 1, king.get_y());
            king.set_coord(king.get_x() - 2, king.get_y());
        }
        return true;
    }
    return false;
}

Rock* Board::get_rock(int X, Color col){
    char rock = col ? 'R' : 'r';
    std::vector<std::unique_ptr<Piece>>& pieces = col ? piecesBlack : piecesWhite;
    X = X < 4 ? 0 : 7;

    for (const auto& piece : pieces) {
        if (piece->name == rock && piece->get_x() == X) {
            Rock* rockPtr = dynamic_cast<Rock*>(piece.get());
            return rockPtr;
        }
    }
    return nullptr;
}

King& Board::getKing(Color col) {
    std::vector<std::unique_ptr<Piece>>& pieces = col ? piecesBlack : piecesWhite ;
    for (const auto& piece : pieces) {
        if (piece->name == 'K' || piece->name == 'k') {
            King* kingPtr = dynamic_cast<King*>(piece.get());
            return *kingPtr;
        }
    }
    throw std::runtime_error("King not found");
}

bool Board::isUnderAttack(int X, int Y, Color col, char** position) {
     std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    for (const auto& piece : opponentPieces) {
        
        if (piece->isPosible(X, Y, position, 0)) {
            return true;
        }
    }
    return false;
}

void Board::endGame(){
    piecesBlack.clear();         piecesWhite.clear(); 
    piecesBlack.shrink_to_fit(); piecesWhite.shrink_to_fit();
    moveNum = 0;
}

bool Board::kingIsHaveMove(King king){
    int moves[8][2] = {
        {1, 0},  {1, 1},  {0, 1},  {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };

    for (int i = 0; i < 8; ++i) {
        int newX = king.get_x() + moves[i][0];
        int newY = king.get_y() + moves[i][1];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            if ((position[newY][newX] == ' ' || king.hereMyFigure(newX, newY, position))
                && isUnderAttack(newX, newY, king.col, position)) {
                return true; 
            }
        }
    }
    return false; 
}


