from chess_engine.board import Chess_Board
from chess_engine.move import Move, Move_type
from chess_engine.pieces import *
from data_tests_spec_moves import TEST_SHEME_CASTLES
import pytest

def test_en_passant():
    board = Chess_Board()
    pawn = Pawn('P', 3, 0)
    board.board[3][0] = pawn
    expected_enp_x, expected_enp_y = 2, 1
    board.play_turn(Move(1, 1, 3, 1, Move_type.NORMAL)) # move enemy pawn in 2 squers
    move = pawn.get_combat_moves(board.board)[0]
    board.play_turn(move)

    assert board.board[3][0] is None
    assert board.board[3][1] is None
    assert board.board[2][1].name == "P"

def test_pawn_promotion():
    board = Chess_Board()
    pawn = Pawn('p', 6, 0)
    board.board[6][0] = pawn
    moves = pawn.get_moves(board.board)

    board.play_turn(moves[0])
    new_piece = board.board[moves[0].to_x][moves[0].to_y]
    assert board.board[6][0] is None
    assert new_piece is not None
    assert any(new_piece.name == name for name in "qrbn") #check if new_piece is some of posoble pieces
    
@pytest.mark.parametrize("test_sheme", TEST_SHEME_CASTLES)
def test_castles(test_sheme):
    board = Chess_Board(test_sheme)
    board.play_turn(Move(1, 1, 2, 1, Move_type.NORMAL))
    king = board.board[7][3]
    moves = king.get_moves(board.board)

    short_castle = [move for move in moves if move.type_ == Move_type.CASTLE and move.to_y == 1]
    long_castle = [move for move in moves if move.type_ == Move_type.CASTLE and move.to_y == 5]

    board.make_move(short_castle[0])
    assert board.board[7][3] is None
    assert board.board[7][1].name == 'K'
    assert board.board[7][2].name == 'R'
    board.undo_move()
    board.make_move(long_castle[0])
    assert board.board[7][3] is None
    assert board.board[7][5].name == 'K'
    assert board.board[7][4].name == 'R'