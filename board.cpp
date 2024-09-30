#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <utility>

using namespace std;

Board::Board(){
    copy(&defaultPosition[0][0], &defaultPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove = 0;
}
Board::Board(const char myPosition[8][8]) {
    copy(&myPosition[0][0], &myPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove= 0;
}

const char Board::defaultPosition[8][8] = {
    {'r', 'h', ' ', 'q', 'k', 'b', 'h', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'H', 'B', 'Q', 'K', 'B', 'H', 'R'}
};

void Board::writeCurrentBoard() {
    cout << "    A B C D E F G H" << std::endl;
    for (int i = 0; i < 8; i++) {
        cout << i + 1 << "   ";
        for (int j = 0; j < 8; j++) {
            cout << position[i][j] << " ";
        }
        cout << std::endl;
    }
}
void Board::startGame(std::vector<std::unique_ptr<Piece>>& piecesWhite,
                        std::vector<std::unique_ptr<Piece>>& piecesBlack){

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            
            switch(position[j][i]){
                case 'P': 
                    piecesBlack.push_back(std::make_unique<Pawn>(i, j, 'P'));
                    break;

                case 'p':  
                    piecesWhite.push_back(std::make_unique<Pawn>(i, j, 'p'));
                    break;

                case 'R':
                    piecesBlack.push_back(std::make_unique<Rock>(i, j, 'R'));
                    break;

                case 'r':
                
                    piecesWhite.push_back(std::make_unique<Rock>(i, j, 'r'));
                    break;

                case 'H':
                    piecesBlack.push_back(std::make_unique<Knight>(i, j, 'H'));
                    break;

                case 'h':
                    piecesWhite.push_back(std::make_unique<Knight>(i, j, 'h'));
                    break;

                case 'B':
                    piecesBlack.push_back(std::make_unique<Bishop>(i, j, 'B'));
                    break;
                    
                case 'b':
                    piecesWhite.push_back(std::make_unique<Bishop>(i, j, 'b'));
                    break;

                case 'Q':
                    piecesBlack.push_back(std::make_unique<Queen>(i, j, 'Q'));
                    break;
                
                case 'q':
                    piecesWhite.push_back(std::make_unique<Queen>(i, j, 'q'));
                    break;

                case 'K':
                    piecesBlack.push_back(std::make_unique<King>(i, j, 'K'));
                    break;

                case 'k':
                    piecesWhite.push_back(std::make_unique<King>(i, j, 'k'));
                    break;
            }
        }
    }
    // for (auto it = piecesWhite.begin(); it != piecesWhite.end(); ++it) {
    //     cout << (*it)->name << endl;
    // }
}

bool Board::isCheck(int H, int V, std::vector<std::unique_ptr<Piece>>& pieces, char (&testPosition)[8][8]){
    
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if((*it)->isPosible(H, V, testPosition, 2)){
            int h = (*it)->coordH;
            int v = (*it)->coordV;
            cout << (*it)->name << endl;
            cout << h << endl;
            cout << v << endl;

            return true;

        }
    }
    return false;
}

bool Board::isCheckMate(std::vector<std::unique_ptr<Piece>>& pieces1,
                        std::vector<std::unique_ptr<Piece>>& pieces2, King &King){
    
    int KingH = King.get_coordH();
    int KingV = King.get_coordV();
    if (!(isCheck(KingH, KingV, pieces1, position))){
        return false;
    }

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Пропустити саму позицію короля
            if (dx == 0 && dy == 0)continue;
            
            int newH = KingH + dx;
            int newV = KingV + dy;
            // Перевіряємо, чи нова позиція в межах дошки
            if (newH >= 0 && newH < 8 && newV >= 0 && newV < 8) continue;
            
            // isCHeck() в діному випадку використовується для перевірки
            // чи перекриває клітинку чужа фігура
            if (!(King.hereMyFigure(newH, newV, position) || isCheck(newH, newV, pieces1, position))) 
                return false;
        }
    }
    char testPosition[8][8];
    copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);
    for (auto it = pieces1.begin(); it != pieces1.end(); ++it) {
        if((*it)->isPosible(KingH, KingV, position, 2)){    

            int attackerH = (*it)->get_coordH();
            int attackerV = (*it)->get_coordV();
            int dH = ( attackerH - KingH) / std::max(1, std::abs(attackerH - KingH));
            int dV = ( attackerH - KingV) / std::max(1, std::abs(attackerV - KingV));

            int H = KingH + dH;
            int V = KingV + dV;
            while (H != attackerH || V != attackerV) {
                if (isCheck(H, V, pieces2, position)){
                    char befor = testPosition[V][H];
                    testPosition[V][H] = 'R';
                    if (!(isCheck(KingH, KingV, pieces1, testPosition))){
                        return false;
                    }
                    testPosition[V][H] = befor;
                }
                H += dH;
                V += dV;
            }
        }
    }
    return true;
}

bool Board::draw(std::vector<std::unique_ptr<Piece>>& pieces, King &King){
    if (isCheck(King.get_coordH(), King.get_coordV(), pieces, position)){
        return false;
    }  
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            // Пропустити саму позицію короля
            if (dx == 0 && dy == 0)continue;
            
            int newH = King.get_coordH() + dx;
            int newV = King.get_coordV() + dy;
            // Перевіряємо, чи нова позиція в межах дошки
            if (newH >= 0 && newH < 8 && newV >= 0 && newV < 8) continue;
            
            // isCHeck() в діному випадку використовується для перевірки
            // чи перекриває клітинку чужа фігура
            if (!(King.hereMyFigure(newH, newV, position) || isCheck(newH, newV, pieces, position))) 
                return false;
        }
    }
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if((*it)->ishaveMove(position)){
            return false;
        }
    }
    return true;
}

void Board::endGame(Color col, std::vector<std::unique_ptr<Piece>> piecesWhite,
    std::vector<std::unique_ptr<Piece>> piecesBlack){


    }

bool Board::move(int currentCoordH, int currentcoordV, int newcoordH, int newCoordV, King King,
                    std::vector<std::unique_ptr<Piece>>& pieces1,
                    std::vector<std::unique_ptr<Piece>>& pieces2){

    char testPosition[8][8];
    copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);
    testPosition[currentcoordV][currentCoordH] = ' ';
    testPosition[newCoordV][newcoordH] = 'R';
    if (isCheck(King.get_coordH(), King.get_coordV(), pieces1, testPosition))
        
       { cout << 'e' << endl; return false;}
    cout << position[currentcoordV][currentCoordH] << endl;
    for (auto & piece : pieces2){

        if (piece->get_coordH() == currentCoordH && piece->get_coordV() == currentcoordV){
            cout << piece->name << endl;
            cout << piece->coordH << endl;
            cout << piece->coordV << endl;

            return piece->pmove(newcoordH, newCoordV, position, numverOfMove);
        }
    }
    return false;
    
}

