#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>
#include <memory>
#include <cmath>




using namespace std;


Board::Board(){
    copy(&defaultPosition[0][0], &defaultPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove = 0;

}
Board::Board(const char myPosition[8][8]) {
    copy(&myPosition[0][0], &myPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove = 0;


}

const char Board::defaultPosition[8][8] = {
    {'r', ' ', ' ', 'q', 'k', 'b', ' ', 'r'},
    {'p', 'p', 'P', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', 'P', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', 'P', 'P', 'P', 'P', 'P', ' '},
    {'R', ' ', ' ', 'K', ' ', ' ', ' ', 'R'}
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

void Board::push_back_piece(char name, int i, int j){                
    switch(name){
        case 'P': 
            piecesBlack.push_back(std::make_unique<Pawn>(i, j, 'P'));
            break;

        case 'p':  
            piecesWhite.push_back(std::make_unique<Pawn>(i, j, 'p'));
            break;

        case 'R':
            piecesBlack.push_back(std::make_unique<Rock>(i, j, 'R'));
            break;

        case 'r':
        
            piecesWhite.push_back(std::make_unique<Rock>(i, j, 'r'));
            break;

        case 'H':
            piecesBlack.push_back(std::make_unique<Knight>(i, j, 'H'));
            break;

        case 'h':
            piecesWhite.push_back(std::make_unique<Knight>(i, j, 'h'));
            break;

        case 'B':
            piecesBlack.push_back(std::make_unique<Bishop>(i, j, 'B'));
            break;
            
        case 'b':
            piecesWhite.push_back(std::make_unique<Bishop>(i, j, 'b'));
            break;

        case 'Q':
            piecesBlack.push_back(std::make_unique<Queen>(i, j, 'Q'));
            break;
        
        case 'q':
            piecesWhite.push_back(std::make_unique<Queen>(i, j, 'q'));
            break;

        case 'K':
            piecesBlack.push_back(std::make_unique<King>(i, j, 'K'));
            break;

        case 'k':
            piecesWhite.push_back(std::make_unique<King>(i, j, 'k'));
            break;
    }
}

void Board::startGame(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            push_back_piece(position[j][i], i, j);
        }
    }
    kingWhite = &getKing(WHITE);
    kingBlack = &getKing(BLACK);
}

bool Board::isCheckMate(Color col) {
    King& king = col ? (*kingBlack) : (*kingWhite);
    int kingX = king.get_x();
    int kingY = king.get_y();
    if (isUnderAttack(kingX, kingY, col, position)) return false;

    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    
    char testPosition[8][8];
    std::copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);

    for (auto& piece : opponentPieces) {
        int attackerX = piece->get_x();
        int attackerY = piece->get_y();

        
        if (piece->isPosible(kingX, kingY, position, 2)) {
            
            int dX = (attackerX - kingX) / std::max(1, std::abs(attackerX - kingX));
            int dY = (attackerY - kingY) / std::max(1, std::abs(attackerY - kingY));

            
            int X = kingX + dX;
            int Y = kingY + dY;

            
            while (X != attackerX || Y != attackerY) {
                if (isUnderAttack(X, Y, col, position)) {
                    char before = testPosition[Y][X];
                    testPosition[Y][X] = ' '; 
                    if (isUnderAttack(kingX, kingY, col, testPosition)) {
                        testPosition[Y][X] = before; 
                        return false;
                    }
                    testPosition[Y][X] = before;
                }
                X += dX;
                Y += dY;
            }
        }
    }

    return king.ishaveMove(position); 
}

bool Board::draw(Color col) {
    King& king = col ? (*kingBlack) : (*kingWhite);

    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    if (isUnderAttack(king.get_x(), king.get_y(), col, position)) {
        return false; 
    }
    for (auto& piece : myPieces) {
        if (piece->ishaveMove(position)) {
            return false; 
        }
    }
    return true;
}

bool Board::move(int currentX, int currentY, int newX, int newY, Color col, char newName) {  
    King& king = col ? (*kingBlack) : (*kingWhite); 
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    char testPosition[8][8];
    std::copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);
    testPosition[currentY][currentX] = ' ';
    testPosition[newY][newX] = 'R';

    if (isUnderAttack(king.get_x(), king.get_y(), col, testPosition)) return false;
    
    
    for (auto piece = myPieces.begin(); piece != myPieces.end(); ++piece) {
        if ((*piece)->get_x() == currentX && (*piece)->get_y() == currentY) {
            //castle
            if (king.get_x() == currentX && king.get_y() == currentY){
                if(abs(king.get_x() - newX) == 2){
                    return castle(king, get_rock(newX, col), col);
                }
            }
            //promote
            if ((*piece)->move(newX, newY, position, 2)) {
                
                if (((*piece)->name == 'P' || (*piece)->name == 'p') && (newY == 7 || newY == 0)) {
                    
                    piece = myPieces.erase(piece);
                    push_back_piece(newName, newX, newY);
                    position[newY][newX] = newName;
                } 

                for (auto it = opponentPieces.begin(); it != opponentPieces.end(); ) {
                    if ((*it)->get_x() == newX && (*it)->get_y() == newY) {
                        it = opponentPieces.erase(it);
                    } else {
                        ++it;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

Stun Board::check_position(Color col){
    if (isCheckMate(col)) return col ? WINBLACK : WINWHITE;
    if (draw(col)) return DRAW;
    return PLAY;
}

bool Board::set_promote(){
    
    for (auto it = piecesWhite.begin(); it != piecesWhite.end(); ) {
        if ((*it)->name == 'P' && (*it)->get_y() == 7) {  
            char newPiece;
            std::cout << "Promote your pawn! Choose Q (Queen), R (Rook), B (Bishop), or N (Knight): ";
            std::cin >> newPiece;

            push_back_piece(newPiece, (*it)->get_x(), (*it)->get_y());  
            it = piecesWhite.erase(it);  
        } else {
            ++it;  
        }
    }


    for (auto it = piecesBlack.begin(); it != piecesBlack.end(); ) {
        if ((*it)->name == 'p' && (*it)->get_y() == 0) {  
            char newPiece;
            std::cout << "Promote your pawn! Choose Q (Queen), R (Rook), B (Bishop), or N (Knight): ";
            std::cin >> newPiece;

            push_back_piece(newPiece, (*it)->get_x(), (*it)->get_y());  
            it = piecesBlack.erase(it);  
        } else {
            ++it; 
        }
    }

    return true;  
}

bool Board::canCastle(King &king, Rock &rook, Color col) {
    // Перевіряємо, чи король і тура не рухалися
    if (king.isHasMoved()|| rook.isHasMoved() ||
        isUnderAttack(king.get_x(), king.get_y(), col, position)) {
            cout << "hm";
            return false;
        }
    int startX = king.get_x() < rook.get_x() ? king.get_x() : rook.get_x();
    int endX = king.get_x() > rook.get_x() ? king.get_x() : rook.get_x();

    for (int i = startX + 1; i < endX; ++i) {
        if (position[king.get_y()][i] != ' '||
        isUnderAttack( i, king.get_y(), col, position)){
            cout << king.get_x() << " " << i << endl;
            return false;
        }  // Якщо на шляху є фігура
    }
    cout << "Xm";
    return true; 
}

bool Board::castle(King &king, Rock *rook, Color col) {
    if (rook == nullptr) return false;

    if (canCastle(king, (*rook), col)) {
        if ((*rook).get_x() > king.get_x()) {
            // Рокіровка на праву сторону
            position[king.get_y()][king.get_x() + 1] = (*rook).name;
            position[king.get_y()][king.get_x() + 2] = king.name;
            position[rook->get_y()][rook->get_x()] = position[king.get_y()][king.get_x()] = ' ';

            (*rook).set_coord(king.get_x() + 1, king.get_y());
            king.set_coord(king.get_x() + 2, king.get_y());
        } else { 

            position[king.get_y()][king.get_x() - 1] = (*rook).name;
            position[king.get_y()][king.get_x() - 2] = king.name;
            position[rook->get_y()][rook->get_x()] = position[king.get_y()][king.get_x()] = ' ';
            (*rook).set_coord(king.get_x() - 1, king.get_y());
            king.set_coord(king.get_x() - 2, king.get_y());

        }
        return true;
    }
    return false;
}

Rock* Board::get_rock(int X, Color col){
    char rock = col ? 'R' : 'r';
    std::vector<std::unique_ptr<Piece>>& pieces = col ? piecesBlack : piecesWhite;
    X = X < 4 ? 0 : 7;

    for (const auto& piece : pieces) {
        if (piece->name == rock && piece->get_x() == X) {
            Rock* rockPtr = dynamic_cast<Rock*>(piece.get());
            return rockPtr;
        }
    }
    return nullptr;
}

King& Board::getKing(Color col) {
    std::vector<std::unique_ptr<Piece>>& pieces = col ? piecesBlack : piecesWhite ;
    for (const auto& piece : pieces) {
        if (piece->name == 'K' || piece->name == 'k') {
            King* kingPtr = dynamic_cast<King*>(piece.get());
            return *kingPtr;
        }
    }
    throw std::runtime_error("King not found");
}

bool Board::isUnderAttack(int X, int Y, Color col, char (&position)[8][8]) {
     std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    for (const auto& piece : opponentPieces) {
        
        if (piece->isPosible(X, Y, position, 0)) {
            cout << piece->name << "  "<< piece->get_x() << piece->get_y() << endl;
            return true;
        }
    }
    return false;
}
