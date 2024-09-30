#include "board.hpp"
#include <vector>
#include "pieces.hpp"

using namespace std;

int main(){
    Board board;
    std::vector<std::unique_ptr<Piece>> piecesWhite;
    std::vector<std::unique_ptr<Piece>> piecesBlack;
    board.writeCurrentBoard();
    board.startGame(piecesWhite, piecesBlack);
    


    // while(true){
        
    // }

    return 0;
}

