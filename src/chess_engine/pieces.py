from .move import Move, Move_type

class _Piece:
    """Base class for every piece"""
    def __repr__(self):
        return f"'{self.name}, x: {self.x}, y: {self.y}'"
    
    def __str__(self):
        return self.name
    
    def __eq__(self, other):
        if other is None:
            return False
        return(
            self.x == other.x and
            self.y == other.y and
            self.color == other.color and
            self.type == other.type and
            self.has_moved == other.has_moved
        )    
    def __init__(self, name, x, y):
        self.x = x
        self.y = y
        self.color = "black" if name.isupper() else "white"
        self.name = name
        self.has_moved = False
        self.type = (self.__class__.__name__).lower()

    def set_position(self, x, y):
        self.x = x
        self.y = y
        self.has_moved = True

    def is_ally(self, color):
        return self.color == color

    def get_moves(self, board):
        pass

    def get_combat_moves(self, board):
        return self.get_moves(board)

class _SlidingPiece(_Piece):
    """class for Queen, Rook, Bishop. their movemnent very similar"""
    direction = ()

    def get_moves(self, board):
        moves = []

        for d_x, d_y in self.direction:
            cur_x = self.x + d_x
            cur_y = self.y + d_y

            while 0 <= cur_x <= 7 and 0 <= cur_y <= 7:
                piece = board[cur_x][cur_y]
                if piece == None:
                    moves.append(Move(self.x, self.y, cur_x, cur_y, Move_type.NORMAL))
                else:
                    if not self.is_ally(piece.color):
                        moves.append(Move(self.x, self.y, cur_x, cur_y, Move_type.NORMAL))
                    break
                cur_x += d_x
                cur_y += d_y
        
        return moves 

class Rook(_SlidingPiece):
    direction = ((0, 1), (1, 0), (0, -1), (-1, 0)) #right, down, left, up
    
class Bishop(_SlidingPiece):
    direction = ((1, 1), (1, -1), (-1, 1), (-1, -1)) # down-right, down-left, up-right, up-left
        
class Queen(_SlidingPiece):
    direction = ((0, 1), (1, 0), (0, -1), (-1, 0), 
                (1, 1), (1, -1), (-1, 1), (-1, -1))

class Pawn(_Piece):
    en_passant_target = None

    def __init__(self, name, x, y):
        super().__init__(name, x, y)
        self.direction = -1 if self.color == "black" else 1
        self.promotion_pieces = ['Q', 'R', 'B', 'N'] \
            if self.color == "black" else ['q', 'r', 'b', 'n']

    def get_moves(self, board):
        moves = []
        start_row = 6 if self.color == "black" else 1
        end_row = 0 if self.color == "black" else 7
        new_x = self.x + self.direction
        m_type = Move_type.PROMOTION if new_x == end_row else Move_type.NORMAL

        if board[new_x][self.y] is None:
            if m_type == Move_type.PROMOTION:
                for piece_name in self.promotion_pieces:
                    moves.append(Move(self.x, self.y, self.x + self.direction, self.y, m_type, piece_name))
            else:
                moves.append(Move(self.x, self.y, self.x + self.direction, self.y, m_type))
        if start_row == self.x and board[self.x + self.direction * 2][self.y] is None:
            moves.append(Move(self.x, self.y, self.x + self.direction * 2, self.y, Move_type.NORMAL))
        
        return moves + self.get_combat_moves(board)

    def get_combat_moves(self, board):
        moves = []
        new_x = self.x + self.direction
        end_row = 0 if self.color == "black" else 7
        m_type = Move_type.PROMOTION if new_x == end_row else Move_type.NORMAL
        y1 = self.y + 1
        y2 = self.y - 1

        if y1 <= 7 and board[new_x][y1] is not None and not self.is_ally(board[new_x][y1].color):
            if m_type == Move_type.PROMOTION:
                for piece_name in self.promotion_pieces:
                    moves.append(Move(self.x, self.y, new_x, y1, m_type, True, piece_name))
            else:
                moves.append(Move(self.x, self.y, new_x, y1, m_type, True))
        if y2 >= 0 and board[new_x][y2] is not None and not self.is_ally(board[new_x][y2].color):
            if m_type == Move_type.PROMOTION:
                for piece_name in self.promotion_pieces:
                    moves.append(Move(self.x, self.y, new_x, y2, m_type, True, piece_name))
            else:
                moves.append(Move(self.x, self.y, new_x, y2, m_type, True))
        if self.en_passant_target is not None:
            if self.en_passant_target.x == self.x and abs(self.en_passant_target.y - self.y) == 1 and not\
                    self.is_ally(self.en_passant_target.color):
                moves.append(Move(self.x, self.y, self.en_passant_target.x + self.direction, self.en_passant_target.y, Move_type.EN_PASSANT, True))
        
        return moves

class Knight(_Piece):
    direction = ((2, 1), (2, -1),(-2, 1), (-2, -1),
                    (1, 2), (1, -2),(-1, 2), (-1, -2))
    
    def __init__(self, name, x, y):
        super().__init__(name, x, y)
        self.can_castling = [False, False]

    def get_moves(self, board):
        moves = []

        for dx, dy in self.direction:
            nx, ny = self.x + dx, self.y + dy
            if 0 <= nx <= 7 and 0 <= ny <= 7:                
                if board[ny][nx] is None or not self.is_ally(board[ny][nx].color):
                    is_capture = True if board[ny][nx] is not None else False
                    moves.append(Move(self.x, self.y, nx, ny, Move_type.NORMAL, is_capture))

        return moves

class King(_Piece):
    direction = ((0, 1), (1, 0), (0, -1), (-1, 0), 
                (1, 1), (1, -1), (-1, 1), (-1, -1))
    
    def __init__(self, name, x, y):
        super().__init__(name, x, y)
        self.can_castling = [False, False]


    def get_moves(self, board):
        moves = []

        for dx, dy in self.direction:
            nx, ny = self.x + dx, self.y + dy
            if 0 <= nx <= 7 and 0 <= ny <= 7:
                if board[ny][nx] is None or not self.is_ally(board[ny][nx].color):
                    is_capture = True if board[ny][nx] is not None else False
                    moves.append(Move(self.x, self.y, nx, ny, Move_type.NORMAL))
        
        if self.can_castling[0]:
            moves.append(Move(self.x, self.y, self.x, self.y - 2, Move_type.CASTLE))

        if self.can_castling[1]:
            moves.append(Move(self.x, self.y, self.x, self.y + 2, Move_type.CASTLE))

        return moves
    
    def set_castling(self, short, long):
        self.can_castling = [short, long]
        