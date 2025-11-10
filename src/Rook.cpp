#include "Rook.h"

Rook::Rook(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/rookW3.png"
                : "../assets/pieces/rookB3.png") {}
