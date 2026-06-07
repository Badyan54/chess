from .board import Chess_Board, Status
from .move import Move, Move_type
from .pieces import *

board = Chess_Board()
moves = board.get_valid_piece_moves(board.board[1][0])
board.play_turn(moves[0])
moves = board.get_valid_piece_moves(board.board[6][5])
board.play_turn(moves[0])
moves = board.get_valid_piece_moves(board.board[1][4])
board.play_turn(moves[0])
m = Move_type.NORMAL
moves = board.get_valid_piece_moves(board.board[7][4])
board.play_turn(moves[2])
board.play_turn(Move(6, 4, 5, 4, m))
board.play_turn(Move(7, 5, 6, 4, m))
board.play_turn(Move(7, 6, 5, 7, m))
board.play_turn(Move(7, 3, 7, 5, Move_type.CASTLE))
board.play_turn(Move(6, 0, 4, 0, m))
board.play_turn(Move(4, 0, 3, 0, m))
board.play_turn(Move(1, 1, 3, 1, m))
moves = board.get_valid_piece_moves(board.board[3][0])
board.play_turn(moves[0])

board.play_turn(Move(2, 1, 1, 2, m))
print(board.transform_to_notation(Move(2, 1, 1, 2, m, True), Status.OK))



# if moves != []:
#     board.play_turn(moves[0])



