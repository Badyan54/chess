from collections import namedtuple
from enum import Enum, auto
from .pieces import *

En_pass_info = namedtuple("En_pass_info", ["x", "y", "color"])

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

    def render(self):
        print("0", end='  ')
        for i in range(1, 9):
            print(str(i), end=' ')
        print()
        print()
        for index, row in enumerate(self.board):
            print(index + 1, end='  ')
            for squer in row:
                if squer is None:
                    squer = ' '
                print(f"{squer} ", end='')
            print()
        print()
    
    def set_moves(self):
        moves_table = {}
        reversed_moves = defaultdict(list)
        for_board = copy.deepcopy(self.board)
        for row in for_board:
            for piece in row:
                if piece is not None:
                    cords = (piece.x, piece.y)
                    moves = self.get_valid_piece_moves(piece)
                    moves_table[cords] = moves
                    for move in moves:
                        reversed_moves[(piece.name, move.to_x, move.to_y)].append(move)

        self.moves = moves_table
        self.reversed_moves = reversed_moves

    def get_valid_piece_moves(self, piece):
        moves = piece.get_moves(self.board)
        valid_moves = []

        for move in moves:
            self.make_move(move)
            if not self.is_check(self.board, piece.color):
                valid_moves.append(move)
            self.undo_move()             
        return valid_moves
    
    def get_moves(self):
        return self.moves
    
    def make_move(self, move):
        pass

    def undo_move(self):
        pass

    def set_en_passant_target(self):
        Pawn.en_passant_target = None
        if self.last_move != {}:
            if self.last_move[1] == "pawn" and abs(self.last_move[0].from_x - self.last_move[0].to_x) == 2:
                Pawn.en_passant_target = En_pass_info(
                    x = self.last_move[0].to_x,
                    y = self.last_move[0].to_y,
                    color = self.last_move[2]
                    )
    
    def is_check(self, board, color):
        king = self.kings[color]
        return self.is_squer_attacked(board, color, king.x, king.y)

    def is_squer_attacked(self, board, color, x, y):
        for row in board:
            for piece in row:
                if piece is not None:
                    if color == piece.color:
                        continue
                    moves = piece.get_combat_moves(board)

                    for attack_x, attack_y in moves:
                        if attack_x == x and attack_y == y:
                            return True

        return False
    
    def is_check_mate(self):
        if not self.is_check(self.board, self.whose_turn):
            return False
        
        king = self.kings[self.whose_turn]
        squers_available = king.get_moves(self.board)
        for x, y in squers_available:
            if not self.is_squer_attacked(self.board, self.whose_turn, x, y):
                return False

        return True

    def is_stalemate(self):
        if self.is_check(self.board, self.whose_turn):
            return False
            
        king = self.kings[self.whose_turn]
        squers_available = king.get_moves(self.board)
        for x, y in squers_available:
            if not self.is_squer_attacked(self.board, self.whose_turn, x, y):
                return False

        return True