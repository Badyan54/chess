from enum import Enum, auto

class Move_type(Enum):
    NORMAL = auto()
    EN_PASSANT = auto()
    CASTLE = auto()
    PROMOTION = auto()

class Move:
    def __repr__(self):
        return f"M {self.to_x}, {self.to_y}"

    def __init__(self, from_x, from_y, to_x, to_y, type_, is_capture=False, promotion_piece=None):
        self.from_x = from_x
        self.from_y = from_y
        self.to_x = to_x
        self.to_y = to_y
        self.type_ = type_
        self.is_capture = is_capture
        self.promotion_piece=promotion_piece
    
    def __iter__(self):
        yield self.to_x
        yield self.to_y