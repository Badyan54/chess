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
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
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
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    int kingH = king.get_coordH();
    int kingV = king.get_coordV();


    if (king.isUnderAttack(kingH, kingV, opponentPieces, position)) {
        return false; 
    }

    char testPosition[8][8];
    std::copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);

    for (auto& piece : opponentPieces) {
        int attackerH = piece->get_coordH();
        int attackerV = piece->get_coordV();

        
        if (piece->isPosible(kingH, kingV, position, 2)) {
            
            int dH = (attackerH - kingH) / std::max(1, std::abs(attackerH - kingH));
            int dV = (attackerV - kingV) / std::max(1, std::abs(attackerV - kingV));

            
            int H = kingH + dH;
            int V = kingV + dV;

            
            while (H != attackerH || V != attackerV) {
                if (king.isUnderAttack(H, V, opponentPieces, position)) {
                    char before = testPosition[V][H];
                    testPosition[V][H] = ' '; 
                    if (king.isUnderAttack(kingH, kingV, opponentPieces, testPosition)) {
                        testPosition[V][H] = before; 
                        return false;
                    }
                    testPosition[V][H] = before;
                }
                H += dH;
                V += dV;
            }
        }
    }

    return king.ishaveMove(position); 
}

bool Board::draw(Color col) {
    King& king = col ? (*kingBlack) : (*kingWhite);
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    if (king.isUnderAttack(king.get_coordH(), king.get_coordV(), opponentPieces, position)) {
        return false; 
    }
    for (auto& piece : myPieces) {
        if (piece->ishaveMove(position)) {
            return false; 
        }
    }
    return true;
}

bool Board::move(int currentCoordH, int currentCoordV, int newCoordH, int newCoordV, Color col, char newName) {
    
    King& king = col ? (*kingBlack) : (*kingWhite);
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    char testPosition[8][8];
    std::copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);
    testPosition[currentCoordV][currentCoordH] = ' ';
    testPosition[newCoordV][newCoordH] = 'R';

   
    if (king.isUnderAttack(king.get_coordH(), king.get_coordV(), opponentPieces, testPosition)) {
        cout << "wtf" << endl;
        return false;
    }
    for (auto piece = myPieces.begin(); piece != myPieces.end(); ++piece) {
        if ((*piece)->get_coordH() == currentCoordH && (*piece)->get_coordV() == currentCoordV) {
            if (king.get_coordH() == currentCoordH && king.get_coordV() == currentCoordV){
                if(abs(king.get_coordH() - newCoordH) == 2){

                    return castle(king, get_rock(newCoordH, myPieces, col), opponentPieces);
                }
            }
            if ((*piece)->move(newCoordH, newCoordV, position, 2)) {
                
                if (((*piece)->name == 'P' || (*piece)->name == 'p') && (newCoordV == 7 || newCoordV == 0)) {
                    
                    piece = myPieces.erase(piece);
                    push_back_piece(newName, newCoordH, newCoordV);
                    position[newCoordV][newCoordH] = newName;
                } 

                for (auto it = opponentPieces.begin(); it != opponentPieces.end(); ) {
                    if ((*it)->get_coordH() == newCoordH && (*it)->get_coordV() == newCoordV) {
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

Stun Board::check_position(Color col){
    if (isCheckMate(col)) return col ? WINBLACK : WINWHITE;
    if (draw(col)) return DRAW;
    return PLAY;
}

bool Board::set_promote(){
    
    for (auto it = piecesWhite.begin(); it != piecesWhite.end(); ) {
        if ((*it)->name == 'P' && (*it)->get_coordV() == 7) {  
            char newPiece;
            std::cout << "Promote your pawn! Choose Q (Queen), R (Rook), B (Bishop), or N (Knight): ";
            std::cin >> newPiece;

            push_back_piece(newPiece, (*it)->get_coordH(), (*it)->get_coordV());  
            it = piecesWhite.erase(it);  
        } else {
            ++it;  
        }
    }


    for (auto it = piecesBlack.begin(); it != piecesBlack.end(); ) {
        if ((*it)->name == 'p' && (*it)->get_coordV() == 0) {  
            char newPiece;
            std::cout << "Promote your pawn! Choose Q (Queen), R (Rook), B (Bishop), or N (Knight): ";
            std::cin >> newPiece;

            push_back_piece(newPiece, (*it)->get_coordH(), (*it)->get_coordV());  
            it = piecesBlack.erase(it);  
        } else {
            ++it; 
        }
    }

    return true;  
}

bool Board::canCastle(King &king, Rock &rook, std::vector<std::unique_ptr<Piece>>& opponentPieces) {
    // Перевіряємо, чи король і тура не рухалися
    if (king.hasMoved || rook.hasMoved ||
        king.isUnderAttack(king.get_coordH(), king.get_coordV(), opponentPieces, position)) {
            cout << "hm";
            return false;
        }
    int startH = king.coordH < rook.coordH ? king.coordH : rook.coordH;
    int endH = king.coordH > rook.coordH ? king.coordH : rook.coordH;

    for (int i = startH + 1; i < endH; ++i) {
        if (position[king.coordV][i] != ' '||
        king.isUnderAttack( i, king.get_coordV(), opponentPieces, position)){
            cout << king.get_coordH() << " " << i << endl;
            return false;
        }  // Якщо на шляху є фігура
    }
    cout << "hm";
    return true; 
}

bool Board::castle(King &king, Rock *rook, std::vector<std::unique_ptr<Piece>>& opponentPieces) {
    if (rook == nullptr) return false;

    if (canCastle(king, (*rook), opponentPieces)) {
        if ((*rook).coordH > king.coordH) {
            // Рокіровка на праву сторону
            position[king.coordV][king.coordH + 1] = (*rook).name;
            position[king.coordV][king.coordH + 2] = king.name;
            position[rook->coordV][rook->coordH] = position[king.coordV][king.coordH] = ' ';

            (*rook).set_coord(king.coordH + 1, king.coordV);
            king.set_coord(king.coordH + 2, king.coordV);
        } else { 

            position[king.coordV][king.coordH - 1] = (*rook).name;
            position[king.coordV][king.coordH - 2] = king.name;
            position[rook->coordV][rook->coordH] = position[king.coordV][king.coordH] = ' ';
            (*rook).set_coord(king.coordH - 1, king.coordV);
            king.set_coord(king.coordH - 2, king.coordV);

        }
        return true;
    }
    return false;
}


Rock* Board::get_rock(int H, std::vector<std::unique_ptr<Piece>>& pieces, Color col){
    char rock = col ? 'R' : 'r';
    H = H < 4 ? 0 : 7;

    for (const auto& piece : pieces) {
        if (piece->name == rock && piece->get_coordH() == H) {
            Rock* rockPtr = dynamic_cast<Rock*>(piece.get());
            return rockPtr;
        }
    }
    // cout << "hm" << endl;
    return nullptr;
}

