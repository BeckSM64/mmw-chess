#include "Bishop.h"

Bishop::Bishop(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/bishopW3.png"
                : "../assets/pieces/bishopB3.png") {}
