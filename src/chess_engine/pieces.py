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