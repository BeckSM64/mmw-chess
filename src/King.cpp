#include "King.h"

King::King(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/kingW3.png"
                : "../assets/pieces/kingB3.png") {}
