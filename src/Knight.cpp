#include "Knight.h"

Knight::Knight(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/knightW3.png"
                : "../assets/pieces/knightB3.png") {}
