#include "board.hpp"
#include "pieces.hpp"

using namespace std;

int main(){
    
    Board board;

    board.startGame();
    board.writeCurrentBoard();
    board.move(3, 1, 3, 2, WHITE, ' ');

    board.move(3, 7, 5, 7, BLACK, ' ');
    board.move(4, 1, 4, 3, WHITE, ' ');
    board.move(5, 3, 4, 2, BLACK, ' ');
    board.move(2, 1, 2, 0, BLACK, 'Q');
    board.move(5, 7, 6, 7, BLACK, ' ');
    board.writeCurrentBoard();
    return 0;
}

