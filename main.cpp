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
    board.move(1, 7, 2, 5, king, piecesWhite, piecesBlack, ' ');
    // board.writeCurrentBoard();
    board.move(1, 6, 1, 5, king, piecesWhite, piecesBlack, ' ');
    board.writeCurrentBoard();
    board.move(2, 1, 2, 0, king, piecesWhite, piecesBlack, 'Q');
    board.writeCurrentBoard();
    board.move(2, 0, 2, 2, king, piecesWhite, piecesBlack, ' ');
    board.writeCurrentBoard();
        board.move(2, 2, 3, 3, king, piecesWhite, piecesBlack, ' ');
    board.writeCurrentBoard();
    cout << piecesBlack.back()->name<< piecesBlack.back()->get_coordH() << "  " << piecesBlack.back()->get_coordV() << endl;




    
    


    // while(true){
        
    // }

    return 0;
}

