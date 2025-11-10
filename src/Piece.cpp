#include "Piece.h"
#include <iostream>

Piece::Piece(PieceColor color, sf::Vector2i position, const std::string& texturePath)
    : color(color), position(position), texturePath(texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

void Piece::draw(sf::RenderWindow& window) {
    sprite.setPosition(position.x, position.y);
    window.draw(sprite);
}

sf::Vector2i Piece::getPosition() const { return position; }
void Piece::setPosition(sf::Vector2i newPos) { position = newPos; }
PieceColor Piece::getColor() const { return color; }
