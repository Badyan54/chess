from chess_engine.move import Move
from chess_engine.pieces import *
import pytest

TEST_DATA_DIRECTION = [
    pytest.param(Rook('r', 0, 0),
    ((0, 1), (1, 0), (0, -1), (-1, 0)),
    id="rook-dir"
    ),
    pytest.param(
        Bishop('b', 0, 2),
        ((1, 1), (1, -1), (-1, 1), (-1, -1)),
        id="bishop-dir"
    ),
    pytest.param(
        Queen('q', 0, 4),
        ((0, 1), (1, 0), (0, -1), (-1, 0),(1, 1), (1, -1), (-1, 1), (-1, -1)),
        id="queen-dir"
    ),
    pytest.param(
        Knight('n', 0, 1),
        ((2, 1), (2, -1), (-2, 1), (-2, -1), (1, 2), (1, -2), (-1, 2), (-1, -2)),
        id="knight-dir"
    ),
    pytest.param(
        King('k', 0, 3),
        ((0, 1), (1, 0), (0, -1), (-1, 0),(1, 1), (1, -1), (-1, 1), (-1, -1)),
        id="king-dir"
    ),
    pytest.param(Pawn('p', 1, 0), 1, id="pawn1-dir"),
    pytest.param(Pawn('P', 6, 0), -1, id="pawn2-dir")
]

TEST_DATA_ON_EMPTY_BOARD = [
    pytest.param(
        Rook('r', 5, 5),
        [(5, 6), (5, 7), (6, 5), (7, 5), (5, 4), (5, 3), (5, 2),
         (5, 1), (5, 0), (4, 5), (3, 5), (2, 5), (1, 5), (0, 5)], #to_x to_y
        id="rook-empty" 
    ),
    pytest.param(
        Bishop('b', 5, 5),
        [(6, 6), (7, 7), (6, 4), (7, 3), (4, 6), (3, 7), (4, 4),
         (3, 3), (2, 2), (1, 1), (0, 0)],
        id="bishop-empty"
    ),
    pytest.param(
        Queen('q', 5, 5),
        [(5, 6), (5, 7), (6, 5), (7, 5), (5, 4), (5, 3), (5, 2),
         (5, 1), (5, 0), (4, 5), (3, 5), (2, 5), (1, 5), (0, 5),
         (6, 6), (7, 7), (6, 4), (7, 3), (4, 6), (3, 7), (4, 4),
         (3, 3), (2, 2), (1, 1), (0, 0)],
         id="queen-empty"
    ),
    pytest.param(
        King('k', 0, 3),
        [(0, 4), (1, 3), (0, 2), (1, 4), (1, 2)],
        id="king-empty"
    ),
    pytest.param(
        Knight('n', 4, 4),
        [(6, 5), (6, 3), (2, 5), (2, 3), (5, 6), (5, 2), (3, 6),
         (3, 2)],
        id="knight-empty"
    ),
    pytest.param(
        Pawn('p', 1, 0),
        [(2, 0), (3, 0)],
        id="pawn-empty"
    )
]

TEST_DATA_DONT_HIT_ALLIES = [
    pytest.param(Rook('r', 5, 4), id="rook-hit"),
    pytest.param(Bishop('b', 4, 4), id="bishop-hit"),
    pytest.param(Queen('q', 4, 4), id="queen-hit"),
    pytest.param(King('k', 4, 4), id="king-hit"),
    pytest.param(Knight('n', 7, 6), id="knight-hit"),
    pytest.param(Pawn('p', 4, 4), id="pawn-hit")
]

TEST_DATA_CANNOT_JUMP_OVER_ANY_PIECE = [
    pytest.param(Pawn('P', 5, 5)),
    pytest.param(Pawn('p', 5, 5))
]