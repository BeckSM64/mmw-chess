#include "Queen.h"

Queen::Queen(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/queenW3.png"
                : "../assets/pieces/queenB3.png") {}
