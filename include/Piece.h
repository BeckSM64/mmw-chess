#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PieceColor { White, Black };

class Piece {
public:
    Piece(PieceColor color, sf::Vector2i position, const std::string& texturePath);
    virtual ~Piece() = default;

    virtual void draw(sf::RenderWindow& window);

    sf::Vector2i getPosition() const;
    void setPosition(sf::Vector2i newPos);
    PieceColor getColor() const;

protected:
    PieceColor color;
    sf::Vector2i position;
    std::string texturePath;
    sf::Texture texture;
    sf::Sprite sprite;
};
