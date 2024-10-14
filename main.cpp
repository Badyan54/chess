#include "board.hpp"
#include <vector>
#include "pieces.hpp"
#include <iostream>

using namespace std;

int main(){
    
    Board board;

    board.startGame();
    // board.move(3, 1, 3, 2, WHITE, ' ');
    board.move(3, 7, 5, 7, BLACK, ' ');
    board.writeCurrentBoard();
    return 0;
}

