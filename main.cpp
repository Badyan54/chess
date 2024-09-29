#include "board.hpp"
#include <vector>
#include "pieces.hpp"

using namespace std;

int main(){
    Board board;
    std::vector<std::unique_ptr<Piece>> pieces;
    board.writeCurrentBoard();
    board.startGame(pieces);

    return 0;
}

