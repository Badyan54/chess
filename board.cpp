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
}
Board::Board(const char myPosition[8][8]) {
    copy(&myPosition[0][0], &myPosition[0][0] + 8 * 8, &position[0][0]);
}

const char Board::defaultPosition[8][8] = {
    {'R', 'k', 'B', 'Q', 'K', 'B', 'k', 'R'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'R', 'k', 'B', 'Q', 'K', 'B', 'k', 'R'}
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
            switch(position[i][j]){
                case 'p':
                    piecesBlack.push_back(std::make_unique<Pawn>(j, i, position[j][i]));
                    break;
                case 'P':            
                    piecesWhite.push_back(std::make_unique<Pawn>(j, i, position[j][i]));
                    break;

                case 'r':
                    piecesBlack.push_back(std::make_unique<Rock>(j, i, position[j][i]));
                    break;
                case 'R':
                    piecesWhite.push_back(std::make_unique<Rock>(j, i, position[j][i]));
                    break;

                case 'h':
                    piecesBlack.push_back(std::make_unique<Knight>(j, i, position[j][i]));
                    break;
                case 'H':
                    piecesWhite.push_back(std::make_unique<Knight>(j, i, position[j][i]));
                    break;

                case 'b':
                    piecesBlack.push_back(std::make_unique<Bishop>(j, i, position[j][i]));
                    break;
                case 'B':
                    piecesWhite.push_back(std::make_unique<Bishop>(j, i, position[j][i]));
                    break;

                case 'q':
                    piecesBlack.push_back(std::make_unique<Queen>(j, i, position[j][i]));
                    break;
                case 'Q':
                    piecesWhite.push_back(std::make_unique<Queen>(j, i, position[j][i]));
                    break;

                case 'k':
                    piecesBlack.push_back(std::make_unique<King>(j, i, position[j][i]));
                    break;
                case 'K':
                    piecesWhite.push_back(std::make_unique<King>(j, i, position[j][i]));
                    break;
            }
        }
    }
}

bool Board::isCheck(int H, int V, std::vector<std::unique_ptr<Piece>>& pieces, char (&testPosition)[8][8]){
    
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if((*it)->isPosible(H, V, testPosition, 2)){
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
                    char befor = testPosition[H][V];
                    testPosition[H][V] = 'R';
                    if (!(isCheck(KingH, KingV, pieces1, testPosition))){
                        return false;
                    }
                    testPosition[H][V] = befor;
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
