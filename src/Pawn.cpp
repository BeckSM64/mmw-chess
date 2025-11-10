#include "Pawn.h"

Pawn::Pawn(PieceColor color, sf::Vector2i position)
    : Piece(color, position,
            color == PieceColor::White
                ? "../assets/pieces/pawnW3.png"
                : "../assets/pieces/pawnB3.png") {}
