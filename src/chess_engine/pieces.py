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