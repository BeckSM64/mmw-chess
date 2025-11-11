#include "Piece.h"
#include <iostream>

Piece::Piece(PieceColor c, const sf::Vector2i& pos, const std::string& texturePath)
    : color(c), position(pos)
{
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
    sprite.setScale(2.0f, 2.0f);
}
