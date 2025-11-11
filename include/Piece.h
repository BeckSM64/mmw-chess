#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PieceColor { White, Black };

class Piece {
public:
    Piece(PieceColor color, const sf::Vector2i& pos, const std::string& texturePath);
    virtual ~Piece() {}

    PieceColor getColor() const { return color; }
    sf::Vector2i getPosition() const { return position; }
    void setPosition(const sf::Vector2i& newPos) { 
        position = newPos; 
        sprite.setPosition(static_cast<float>(newPos.x), static_cast<float>(newPos.y)); 
    }

    sf::Sprite& getSprite() { return sprite; }

protected:
    PieceColor color;
    sf::Vector2i position; 
    sf::Texture texture;
    sf::Sprite sprite;
};
