#include "ChessBoard.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

ChessBoard::ChessBoard(float windowWidth, float windowHeight, float sideMargin) {
    float boardSize = windowHeight;
    tileSize = boardSize / 8.f;
    float boardWidth = tileSize * 8.f;

    boardPosition.x = (windowWidth - boardWidth) / 2.f;
    boardPosition.y = 0.f; // full height, no vertical padding

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));
            square.setPosition(boardPosition.x + x * tileSize, boardPosition.y + y * tileSize);
            square.setFillColor(((x + y) % 2 == 0)
                ? sf::Color(240, 217, 181)
                : sf::Color(181, 136, 99));
            squares[y][x] = square;
        }
    }

    initPieces();
}

ChessBoard::~ChessBoard() {
    for (auto p : pieces) delete p;
}

void ChessBoard::initPieces() {
    // --- Pawns ---
    for (int x = 0; x < 8; ++x) {
        pieces.push_back(new Pawn(
            PieceColor::White,
            sf::Vector2i(boardPosition.x + x * tileSize + tileSize/2,
                         boardPosition.y + 6 * tileSize + tileSize/2)));
        pieces.push_back(new Pawn(
            PieceColor::Black,
            sf::Vector2i(boardPosition.x + x * tileSize + tileSize/2,
                         boardPosition.y + 1 * tileSize + tileSize/2)));
    }

    // --- Rooks ---
    pieces.push_back(new Rook(PieceColor::White, sf::Vector2i(boardPosition.x + 0*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Rook(PieceColor::White, sf::Vector2i(boardPosition.x + 7*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Rook(PieceColor::Black, sf::Vector2i(boardPosition.x + 0*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));
    pieces.push_back(new Rook(PieceColor::Black, sf::Vector2i(boardPosition.x + 7*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));

    // --- Knights ---
    pieces.push_back(new Knight(PieceColor::White, sf::Vector2i(boardPosition.x + 1*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Knight(PieceColor::White, sf::Vector2i(boardPosition.x + 6*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Knight(PieceColor::Black, sf::Vector2i(boardPosition.x + 1*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));
    pieces.push_back(new Knight(PieceColor::Black, sf::Vector2i(boardPosition.x + 6*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));

    // --- Bishops ---
    pieces.push_back(new Bishop(PieceColor::White, sf::Vector2i(boardPosition.x + 2*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Bishop(PieceColor::White, sf::Vector2i(boardPosition.x + 5*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Bishop(PieceColor::Black, sf::Vector2i(boardPosition.x + 2*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));
    pieces.push_back(new Bishop(PieceColor::Black, sf::Vector2i(boardPosition.x + 5*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));

    // --- Queens ---
    pieces.push_back(new Queen(PieceColor::White, sf::Vector2i(boardPosition.x + 3*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new Queen(PieceColor::Black, sf::Vector2i(boardPosition.x + 3*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));

    // --- Kings ---
    pieces.push_back(new King(PieceColor::White, sf::Vector2i(boardPosition.x + 4*tileSize + tileSize/2, boardPosition.y + 7*tileSize + tileSize/2)));
    pieces.push_back(new King(PieceColor::Black, sf::Vector2i(boardPosition.x + 4*tileSize + tileSize/2, boardPosition.y + 0*tileSize + tileSize/2)));
}

void ChessBoard::draw(sf::RenderWindow& window) {
    for (auto& row : squares)
        for (auto& square : row)
            window.draw(square);

    for (auto p : pieces)
        p->draw(window);
}
