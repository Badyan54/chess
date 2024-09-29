#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>
#include <memory>

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
void Board::startGame(std::vector<std::unique_ptr<Piece>>& pieces){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            switch(position[i][j]){
                case 'p':
                case 'P':
                    
                    pieces.push_back(std::make_unique<Pawn>(j, i, position[j][i]));
                    break;

                case 'r':
                case 'R':
                    pieces.push_back(std::make_unique<Rock>(j, i, position[j][i]));
                    break;

                case 'h':
                case 'H':
                    pieces.push_back(std::make_unique<Knight>(j, i, position[j][i]));
                    break;

                case 'b':
                case 'B':
                    pieces.push_back(std::make_unique<Bishop>(j, i, position[j][i]));
                    break;

                case 'q':
                case 'Q':

                    pieces.push_back(std::make_unique<Queen>(j, i, position[j][i]));
                    break;

                case 'k':
                case 'K':
                    pieces.push_back(std::make_unique<King>(j, i, position[j][i]));
                    break;
            }
        }
    }
}