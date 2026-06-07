import copy
from collections import namedtuple, defaultdict
from enum import Enum, auto
from .pieces import *

En_pass_info = namedtuple("En_pass_info", ["x", "y", "color"])

class Status(Enum):
    OK = auto()
    CHECK = auto()
    CHECK_MATE = auto()
    STALEMATE = auto()
    WRONG_TURN = auto()

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
        self.reversed_moves = {} # here key is "to"

        for row in range(8):
            for col in range(8):
                name = sheme[row][col]
                if name is not None:
                    self.board[row][col] = self.create_piece(name, row, col)
        self.find_kings()
        self.set_state()
        self.set_moves()

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
    
    def play_turn(self, move):
        status = self.turn(move)
        if status == Status.WRONG_TURN:
            return (status, None)
        
        notation = self.transform_to_notation(move, status)
        self.set_moves()
        return (status, notation)

    def turn(self, move):
        if self.board[move.from_x][move.from_y] is not None and \
        self.board[move.from_x][move.from_y].color != self.whose_turn:
            return Status.WRONG_TURN
        self.make_move(move)
        self.whose_turn = "white" if self.whose_turn == "black" else "black"
        self.set_state()
        self.render()
        if self.is_stalemate(): return Status.STALEMATE
        if self.is_check_mate(): return Status.CHECK_MATE
        if self.is_check(self.board, self.whose_turn): return Status.CHECK
        return Status.OK
    
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
        piece = self.board[move.from_x][move.from_y]
        state = self.copy_state()
        self.history.append(state)
        self.last_move = [move, piece.type, piece.color]

        if move.type_ == Move_type.NORMAL:
            self.change_position(piece, move)
        elif move.type_ == Move_type.EN_PASSANT:
            self.change_position(piece, move)
            self.board[piece.x - piece.direction][piece.y] = None
        elif move.type_ == Move_type.PROMOTION:
            self.change_position(piece, move)
            new_piece = self.create_piece(move.promotion_piece, move.to_x, move.to_y)
            self.board[move.to_x][move.to_y] = new_piece
        elif move.type_ == Move_type.CASTLE:
            self.change_position(piece, move)
            rook_y = 0 if move.to_y == 1 else 7
            direction = 1 if move.to_y == 1 else -1
            rook = self.board[piece.x][rook_y]
            self.change_position(rook, Move(rook.x, rook.y, rook.x, piece.y + direction, None))

    def change_position(self, piece, move):
        self.board[move.to_x][move.to_y] = self.board[move.from_x][move.from_y]
        self.board[move.from_x][move.from_y] = None
        piece.set_position(move.to_x, move.to_y)

    def copy_state(self):
        return {
            "board": copy.deepcopy(self.board),
            "whose_turn": self.whose_turn,
            "last_move": self.last_move.copy()
        }

    def undo_move(self):
        if not self.history:
            return

        last_state = self.history.pop()
        self.board = copy.deepcopy(last_state["board"])
        self.whose_turn = last_state["whose_turn"]
        self.last_move = last_state["last_move"]

        self.find_kings()

    def set_state(self):
        self.set_en_passant_target()
        self.set_castels()

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
    
    def set_castels(self):
        king = self.kings[self.whose_turn]
        left_rook, right_rook = self.board[king.x][0], self.board[king.x][7]
        if king.has_moved or self.is_check(self.board, self.whose_turn):
            return
        
        long, short = False, False
        if left_rook is not None and not left_rook.has_moved and self.path_clear(king, left_rook):
            short = True
        if right_rook is not None and not right_rook.has_moved and self.path_clear(king, right_rook):
            long = True

        king.set_castling(short, long)

    def path_clear(self, king, rook):
        step = 1 if king.y < rook.y else -1
        y = king.y
        y += step
        while y != rook.y and king.x == rook.x:
            if self.board[king.x][y] is not None:
                return False
            y += step

        y = king.y
        for _ in range(2):
            y += step
            if self.is_squer_attacked(self.board, self.whose_turn, king.x, y):
                return False

        return True
    
    def transform_to_notation(self, move, status):
        if move.type_ == Move_type.CASTLE:
            return "O-O" if move.to_y == 1 else "O-O-O"

        notation = ""
        row_symbols = ('h', 'g', 'f', 'e', 'd', 'c', 'b', 'a')
        piece = self.board[move.to_x][move.to_y]

        if piece.type != "pawn" and not self.reversed_moves[(move.to_x, move.to_y)]:
            notation += piece.name
        
            for i in range(8):
                if self.board[i][move.from_y] != None\
                    and self.board[i][move.from_y].name == piece.name:
                    notation += row_symbols[move.from_x]
            for i in range(8):
                if self.board[move.from_x][i] != None\
                    and self.board[move.from_x][i].name == piece.name:
                    notation += str(move.from_y + 1)
        elif move.is_capture:
            notation += row_symbols[move.from_y]

        if move.is_capture:
            notation += "x"
        
        notation += row_symbols[move.to_y] + str(move.to_x + 1)

        if move.type_ == Move_type.PROMOTION:
            notation += f" = {move.promotion_piece}"
        if status == Status.CHECK:
            notation += '+'
        elif status == Status.CHECK_MATE:
            notation += '#'
        
        return notation
