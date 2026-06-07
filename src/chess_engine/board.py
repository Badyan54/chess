from enum import Enum, auto
from .pieces import *

class Status(Enum):
    OK = auto()
    CHECK = auto()
    CHECK_MATE = auto()
    STALEMATE = auto()
    WRONG_PLAYER_TURN = auto()

default_shame = [
    ['r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'],  #white
    ['p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'],
    [None, None, None, None, None, None, None, None],
    [None, None, None, None, None, None, None, None],
    [None, None, None, None, None, None, None, None],
    [None, None, None, None, None, None, None, None],
    ['P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'],
    ['R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'] #black
]

class Chess_Board:
    def __init__(self, sheme=default_shame):
        self.board = [[None] * 8 for _ in range(8)]
        self.kings = {}
        self.last_move = {}
        self.history = []
        self.whose_turn = "white"
        self.moves = {} # here key is "from"
        self.reversed_moves = {} # here is key "to"

        for row in range(8):
            for col in range(8):
                name = sheme[row][col]
                if name is not None:
                    self.board[row][col] = self.create_piece(name, row, col)
        self.find_kings()

    def create_piece(self, name, x, y):
        if name is None:
            return Pawn(name, x, y)

        viznachnik = name.upper()
        match viznachnik:
            case 'P':
                return Pawn(name, x, y)
            case 'R':
                return Rook(name, x, y)
            case 'N':
                return Knight(name, x, y)
            case 'B':
                return Bishop(name, x, y)
            case 'Q':
                return Queen(name, x, y)
            case 'K':
                return King(name, x, y)

    def find_kings(self):
            self.kings = {}

            for row in self.board:
                for piece in row:
                    if piece is not None and piece.name.upper() == "K":
                        self.kings[piece.color] = piece