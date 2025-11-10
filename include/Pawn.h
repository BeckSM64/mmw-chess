#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(PieceColor color, sf::Vector2i position);
};
