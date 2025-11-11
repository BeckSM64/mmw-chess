#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdint>
#include "Piece.h"

class ChessBoard {
public:
    ChessBoard(float boardWidth, float boardHeight, float padding);

    void draw(sf::RenderWindow& window);
    void initPieces();
    sf::Vector2i getTileAtPixel(float x, float y);
    bool hasPieceAt(int x, int y, uint32_t playerId);
    void applyMove(int fromX, int fromY, int toX, int toY);

    // Small getters added so client can draw highlights without touching internals
    float getTileSize() const { return tileSize; }
    float getBoardX() const { return boardX; }
    float getBoardY() const { return boardY; }

private:
    float boardWidth, boardHeight;
    float tileSize;
    float boardX, boardY;
    float padding;

    std::array<std::array<Piece*, 8>, 8> squares;
};
