#include <iostream>
#include "board.hpp"

using namespace std;

int main(){
    Board board;
    board.writeCurrentBoard();
    board.startGame();

    return 0;
}

