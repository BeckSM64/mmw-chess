#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>
#include "MMW.h"

// ---------------------------
// Utilities
// ---------------------------
void resizeView(sf::RenderWindow& window, sf::View& view) {
    const float targetWidth = 1920.f;
    const float targetHeight = 1080.f;
    float windowRatio = float(window.getSize().x) / window.getSize().y;
    float targetRatio = targetWidth / targetHeight;
    float sizeX = 1.f;
    float sizeY = 1.f;
    float posX = 0.f;
    float posY = 0.f;

    if (windowRatio > targetRatio) {
        sizeX = targetRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else if (windowRatio < targetRatio) {
        sizeY = windowRatio / targetRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

// ---------------------------
// Main
// ---------------------------
int main(int argc, char** argv) {
    uint32_t myId = 1;
    if (argc >= 2) myId = atoi(argv[1]);
    std::cout << "My ID: " << myId << std::endl;

    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    mmw_initialize("127.0.0.1", 5000);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "MMW Chess", sf::Style::Default);
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    window.setView(view);

    const int boardSize = 8;
    const float sideMargin = 200.f; // desired margin space for captured pieces
    const float fullViewW = 1920.f;
    const float fullViewH = 1080.f;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            else if (e.type == sf::Event::Resized) resizeView(window, view);
        }

        // compute board sizing so it never exceeds view height or available width
        float availableW = fullViewW - sideMargin * 2.f;
        float availableH = fullViewH;
        float boardSide = std::min(availableW, availableH); // square board fits both dims
        float tileSize = boardSide / float(boardSize);

        float startX = (fullViewW - boardSide) / 2.f; // centers horizontally (leaves sideMargin if possible)
        float startY = (fullViewH - boardSide) / 2.f; // centers vertically

        sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));

        window.clear(sf::Color(50, 50, 50));
        window.setView(view);

        // Draw background regions for captured pieces
        sf::RectangleShape leftPanel(sf::Vector2f(startX, fullViewH));
        leftPanel.setFillColor(sf::Color(40, 40, 40));
        leftPanel.setPosition(0.f, 0.f);
        window.draw(leftPanel);

        sf::RectangleShape rightPanel(sf::Vector2f(fullViewW - (startX + boardSide), fullViewH));
        rightPanel.setFillColor(sf::Color(40, 40, 40));
        rightPanel.setPosition(startX + boardSide, 0.f);
        window.draw(rightPanel);

        // Draw chessboard centered with margins and no clipping
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                bool isLight = (row + col) % 2 == 0;
                square.setFillColor(isLight ? sf::Color(240, 217, 181)
                                            : sf::Color(181, 136, 99));
                square.setPosition(startX + col * tileSize, startY + row * tileSize);
                window.draw(square);
            }
        }

        window.display();
    }

    mmw_cleanup();
    return 0;
}
