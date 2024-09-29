#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>

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
void Board::startGame(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            switch(position[i][j]){
                case 'p':
                case 'P':
                    Pawn pawn(j, i, position[i][j]);
                    // cout << "           " << pawn.coordH << pawn.coordV;
                    // pawn.move(1, 1, position[1][1] != ' ');
                    // cout << "           " << pawn.coordH << pawn.coordV;
                    // writeCurrentBoard();
                    break;
                // case 'R':
                //     break;
                // case 'k':
                //     break;
                // case 'B':
                //     break;
                // case 'Q':
                //     break;
                // case 'K':
                //     break;
            }
        }
    }
}