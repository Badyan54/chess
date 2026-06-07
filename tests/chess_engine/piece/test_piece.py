from chess_engine.pieces import *
from data_for_tests import *
import pytest

class Test_pieces:
    def test_piece_init(self):
        rook = Rook('r', 0, 1)
        assert rook.x == 0
        assert rook.y == 1
        assert rook.name == 'r'

    @pytest.mark.parametrize("piece, color", [
                            (Pawn('p', 1, 0), "white"),
                            (Pawn('P', 6, 0), "black")
                            ])
    def test_color_determiner(self, piece, color):
        assert piece.color == color

    @pytest.mark.parametrize("piece, direction", TEST_DATA_DIRECTION)
    def test_directions(self, piece, direction):
        """test for right directions"""
        assert piece.direction == direction

    @pytest.mark.parametrize("piece, moves", TEST_DATA_ON_EMPTY_BOARD)
    def test_moves_on_empty_board(self, piece, moves):
        board = [[None]*8 for _ in range(8)]
        received_moves = piece.get_moves(board)

        for (rec_x, rec_y), (x, y) in zip(received_moves, moves):
            assert rec_x == x
            assert rec_y == y

    def test_pawn_combat_moves(self):
        board = [[None]*8 for _ in range(8)]
        board[5][0] = Pawn('p', 5, 0)
        piece = Pawn('P', 6, 1)
        test_moves = (5, 0)
        moves = piece.get_combat_moves(board)

        assert len(moves) == 1
        assert moves[0].to_x == test_moves[0]
        assert moves[0].to_y == test_moves[1]

    @pytest.mark.parametrize("piece", TEST_DATA_DONT_HIT_ALLIES)
    def test_dont_hit_allies(self, piece):
        board = [[None]*8 for _ in range(8)]
        allie = Pawn('p', 5, 5)
        board[5][5] = allie
        moves = piece.get_combat_moves(board)

        assert not any(x == allie.x and y == allie.y for x, y in moves)
    
    # behavior of pieces who could jump (r, b, q) conteined in _sliding_piece
    # then one rook is enough 
    @pytest.mark.parametrize("obstacle_piece", TEST_DATA_CANNOT_JUMP_OVER_ANY_PIECE)
    def test_cannot_jump_over_any_piece(self, obstacle_piece): 
        board = [[None]*8 for _ in range(8)]
        board[5][5] = obstacle_piece
        piece = Rook('r', 5, 4)
        moves = piece.get_moves(board)
        expected_x, expected_y = 5, 6

        assert not any(x == expected_x and y == expected_y for x, y in moves)
    
    
    