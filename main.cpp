#include "board.hpp"
#include <vector>
#include "pieces.hpp"
#include <iostream>

using namespace std;

int main(){
    Board board;
    std::vector<std::unique_ptr<Piece>> piecesWhite;
    std::vector<std::unique_ptr<Piece>> piecesBlack;
    board.writeCurrentBoard();
    board.startGame(piecesWhite, piecesBlack);

    King king(7, 7, 'K');
    King king1(0, 0, 'K');
    // if (board.move(1, 1, 1, 2, king, piecesWhite)){
    //     // cout << "fefHH";
    // }
    board.move(1, 7, 2, 5, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(1, 6, 1, 5, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(1, 5, 1, 4, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(1, 4, 1, 3, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(2,1, 2, 3, king1, piecesBlack, piecesWhite );
    board.writeCurrentBoard();
    board.move(3, 0, 2, 0, king1, piecesBlack, piecesWhite );
    board.writeCurrentBoard();
    board.move(4, 0, 3, 0, king1, piecesBlack, piecesWhite );
    board.writeCurrentBoard();
    board.move(1, 3, 2, 2, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(2, 7, 1, 6, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move(3, 7, 1, 7, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();
    board.move( 1, 7, 1, 6, king, piecesWhite, piecesBlack);
    board.writeCurrentBoard();



    
    


    // while(true){
        
    // }

    return 0;
}

