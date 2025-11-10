#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>
#include "MMW.h"
#include "ChessBoard.h"

void resizeView(sf::RenderWindow& window, sf::View& view) {
    const float targetWidth = 1920.f;
    const float targetHeight = 1080.f;
    float windowRatio = float(window.getSize().x) / window.getSize().y;
    float targetRatio = targetWidth / targetHeight;
    float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;

    if (windowRatio > targetRatio) {
        sizeX = targetRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    } else if (windowRatio < targetRatio) {
        sizeY = windowRatio / targetRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

int main(int argc, char** argv) {
    uint32_t myId = (argc >= 2) ? atoi(argv[1]) : 1;
    std::cout << "My ID: " << myId << std::endl;

    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    mmw_initialize("127.0.0.1", 5000);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "MMW Chess", sf::Style::Default);
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    window.setView(view);

    ChessBoard board(1920.f, 1080.f, 200.f);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            else if (e.type == sf::Event::Resized)
                resizeView(window, view);
        }

        window.clear(sf::Color(50, 50, 50));
        window.setView(view);
        board.draw(window);
        window.display();
    }

    mmw_cleanup();
    return 0;
}
