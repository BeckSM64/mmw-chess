#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "Piece.h"
// Include other pieces later

class ChessBoard {
public:
    ChessBoard(float windowWidth, float windowHeight, float sideMargin);
    ~ChessBoard();  // to delete pieces
    void draw(sf::RenderWindow& window);

private:
    float tileSize;
    sf::Vector2f boardPosition;
    std::array<std::array<sf::RectangleShape, 8>, 8> squares;
    std::vector<Piece*> pieces;  // raw pointers for C++11

    void initPieces();
};
