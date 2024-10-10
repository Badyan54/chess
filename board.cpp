#include "board.hpp"
#include <iostream>
#include <algorithm>
#include "pieces.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <utility>

using namespace std;
enum Stun {PLAY, DRAW, WINWHITE, WINBLACK};


Board::Board(){
    copy(&defaultPosition[0][0], &defaultPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove = 0;
}
Board::Board(const char myPosition[8][8]) {
    copy(&myPosition[0][0], &myPosition[0][0] + 8 * 8, &position[0][0]);
    numverOfMove= 0;
}

const char Board::defaultPosition[8][8] = {
    {'r', 'h', ' ', 'q', 'k', 'b', 'h', 'r'},
    {'p', 'p', 'P', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'H', 'B', 'Q', 'K', 'B', 'H', 'R'}
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
            push_back_piece(position[j][i], i, j, piecesWhite, piecesBlack);
        }
    }
}

bool Board::isCheckMate(Color col) {
    King& king = col ? kingBlack : kingWhite;
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    int kingH = king.get_coordH();
    int kingV = king.get_coordV();


    if (!isCheck(kingH, kingV, opponentPieces, position)) {
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
    King& king = col ? kingBlack : kingWhite;
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
    King& king = col ? kingBlack : kingWhite;
    std::vector<std::unique_ptr<Piece>>& opponentPieces = col ? piecesWhite : piecesBlack;
    std::vector<std::unique_ptr<Piece>>& myPieces = col ? piecesBlack : piecesWhite;

    char testPosition[8][8];
    std::copy(&position[0][0], &position[0][0] + 8 * 8, &testPosition[0][0]);
    testPosition[currentCoordV][currentCoordH] = ' ';
    testPosition[newCoordV][newCoordH] = 'R';

   
    if (king.isUnderAttack(king.get_coordH(), king.get_coordV(), opponentPieces, testPosition)) {
        return false;
    }

    for (auto piece = myPieces.begin(); piece != myPieces.end(); ++piece) {
        if ((*piece)->get_coordH() == currentCoordH && (*piece)->get_coordV() == currentCoordV) {
            if ((*piece)->move(newCoordH, newCoordV, position, 2)) {
                
                if (((*piece)->name == 'P' || (*piece)->name == 'p') && (newCoordV == 7 || newCoordV == 0)) {
                    
                    piece = myPieces.erase(piece);
                    push_back_piece(newName, newCoordH, newCoordV); 
                } else {
                    
                    position[newCoordV][newCoordH] = (*piece)->getSymbol();
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

King& Board::getKing() {
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

