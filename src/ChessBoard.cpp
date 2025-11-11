#include "ChessBoard.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include <SFML/Graphics.hpp>
#include <iostream>

ChessBoard::ChessBoard(float w, float h, float pad)
    : boardWidth(w - 2*pad), boardHeight(h), padding(pad)
{
    tileSize = boardHeight / 8.f;

    // CENTER THE BOARD HORIZONTALLY
    boardX = (boardWidth - 8 * tileSize) / 2.f + padding; // centered
    boardY = 0.f;

    for (int y=0; y<8; ++y)
        for (int x=0; x<8; ++x)
            squares[y][x] = nullptr;
}

void ChessBoard::draw(sf::RenderWindow& window) {
    for (int y=0; y<8; ++y) {
        for (int x=0; x<8; ++x) {
            sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));
            square.setPosition(boardX + x*tileSize, boardY + y*tileSize);
            square.setFillColor((x+y)%2==0 ? sf::Color(240,217,181) : sf::Color(181,136,99));
            window.draw(square);

            if (squares[y][x])
                window.draw(squares[y][x]->getSprite());
        }
    }
}

void ChessBoard::initPieces() {
    // Pawns
    for (int x = 0; x < 8; ++x) {
        squares[1][x] = new Pawn(PieceColor::Black,
            sf::Vector2i(boardX + x*tileSize + tileSize/2,
                         boardY + 1*tileSize + tileSize/2));
        squares[6][x] = new Pawn(PieceColor::White,
            sf::Vector2i(boardX + x*tileSize + tileSize/2,
                         boardY + 6*tileSize + tileSize/2));
    }

    // Rooks
    squares[0][0] = new Rook(PieceColor::Black,
        sf::Vector2i(boardX + 0*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[0][7] = new Rook(PieceColor::Black,
        sf::Vector2i(boardX + 7*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[7][0] = new Rook(PieceColor::White,
        sf::Vector2i(boardX + 0*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));
    squares[7][7] = new Rook(PieceColor::White,
        sf::Vector2i(boardX + 7*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));

    // Knights
    squares[0][1] = new Knight(PieceColor::Black,
        sf::Vector2i(boardX + 1*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[0][6] = new Knight(PieceColor::Black,
        sf::Vector2i(boardX + 6*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[7][1] = new Knight(PieceColor::White,
        sf::Vector2i(boardX + 1*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));
    squares[7][6] = new Knight(PieceColor::White,
        sf::Vector2i(boardX + 6*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));

    // Bishops
    squares[0][2] = new Bishop(PieceColor::Black,
        sf::Vector2i(boardX + 2*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[0][5] = new Bishop(PieceColor::Black,
        sf::Vector2i(boardX + 5*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[7][2] = new Bishop(PieceColor::White,
        sf::Vector2i(boardX + 2*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));
    squares[7][5] = new Bishop(PieceColor::White,
        sf::Vector2i(boardX + 5*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));

    // Queens
    squares[0][3] = new Queen(PieceColor::Black,
        sf::Vector2i(boardX + 3*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[7][3] = new Queen(PieceColor::White,
        sf::Vector2i(boardX + 3*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));

    // Kings
    squares[0][4] = new King(PieceColor::Black,
        sf::Vector2i(boardX + 4*tileSize + tileSize/2, boardY + 0*tileSize + tileSize/2));
    squares[7][4] = new King(PieceColor::White,
        sf::Vector2i(boardX + 4*tileSize + tileSize/2, boardY + 7*tileSize + tileSize/2));
}

sf::Vector2i ChessBoard::getTileAtPixel(float px, float py) {
    int tx = (px - boardX) / tileSize;
    int ty = (py - boardY) / tileSize;
    if (tx<0 || tx>=8 || ty<0 || ty>=8) return sf::Vector2i(-1,-1);
    return sf::Vector2i(tx, ty);
}

bool ChessBoard::hasPieceAt(int x, int y, uint32_t /*playerId*/) {
    return squares[y][x] != nullptr;
}

void ChessBoard::applyMove(int fromX, int fromY, int toX, int toY) {
    squares[toY][toX] = squares[fromY][fromX];
    squares[fromY][fromX] = nullptr;
    if (squares[toY][toX])
        squares[toY][toX]->setPosition(sf::Vector2i(
            static_cast<int>(boardX + toX*tileSize + tileSize/2),
            static_cast<int>(boardY + toY*tileSize + tileSize/2)
        ));
}
