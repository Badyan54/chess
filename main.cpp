#include <clocale>
#include <iostream>
#include "board.hpp"



using namespace std;

int main(){
    setlocale(LC_CTYPE, "ukr");
    
    const char defaultPosition[8][8] = {
    {'r', ' ', 'r', 'k', ' ', ' ', ' ', 'r'},
    {'p', 'P', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', 'B', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', 'P', 'P', 'P', 'P', 'P', ' '},
    {'R', ' ', ' ', 'K', ' ', ' ', ' ', 'R'}
};
    Board board(defaultPosition);

    board.startGame();
    if((board.move(2, 1, 2, 2, WHITE))){
        cout << "Фігура рухається і відкриває свого короля" << endl;
    } else if(!(board.move(3, 7, 5, 7, BLACK))){
        cout << "Рокіровка не працює" << endl;
    } else if(!(board.move(4, 1, 4, 3, WHITE))){
        cout << "Хід на дві клітинки пішаком не працює"<< endl;
    } else if (!(board.move(5, 3, 4, 2, BLACK))){
        cout << "Взяття на прохід не спрацювало" << endl;
    }
    board.move(1, 1, 1, 0, BLACK);
    char** pos = board.get_position();
    //при тесті треба ввести Q;
    if (pos[0][1] != 'Q'){
        cout << "Промоут не працює" << endl;
    }
    else if (!(board.move(3, 0, 5, 0, WHITE))){
        cout << "Рокіровка білих не працює" << endl;
    }else{
        cout << "Все гаразд" << endl;
    }

    board.writeBoard();

    return 0;
}

