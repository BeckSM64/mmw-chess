#include <SFML/Graphics.hpp>
#include <iostream>
#include "MMW.h"
#include "PlayerMove.h"
#include "ChessBoard.h"

ChessBoard board(1920.f, 1080.f, 200.f);
uint32_t myPlayerId = 1;
sf::Vector2i selected(-1,-1);

void onServerMove(void* data) {
    PlayerMove* move = reinterpret_cast<PlayerMove*>(data);
    board.applyMove(move->fromX, move->fromY, move->toX, move->toY);
}

void sendMove(const PlayerMove& move) {
    mmw_publish_raw("moves_in", (void*)&move, sizeof(PlayerMove), MMW_BEST_EFFORT);
}

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
    if (argc >= 2) myPlayerId = atoi(argv[1]);

    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    mmw_initialize("127.0.0.1", 5000);
    mmw_create_subscriber_raw("moves_out", onServerMove);
    mmw_create_publisher("moves_in");

    sf::RenderWindow window(sf::VideoMode(1280,720), "MMW Chess");
    sf::View view(sf::FloatRect(0,0,1920,1080));
    window.setView(view);

    board.initPieces();

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            else if (e.type == sf::Event::Resized) resizeView(window, view);
            else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i boardCoords = board.getTileAtPixel(mousePos.x, mousePos.y);

                if (boardCoords.x != -1) {
                    if (selected.x == -1) {
                        if (board.hasPieceAt(boardCoords.x, boardCoords.y, myPlayerId))
                            selected = boardCoords;
                    } else {
                        PlayerMove move;
                        move.playerId = myPlayerId;
                        move.fromX = selected.x;
                        move.fromY = selected.y;
                        move.toX = boardCoords.x;
                        move.toY = boardCoords.y;

                        sendMove(move);
                        selected = sf::Vector2i(-1,-1);
                    }
                }
            }
        }

        window.clear(sf::Color(50,50,50));
        board.draw(window);
        window.display();
    }

    mmw_cleanup();
    return 0;
}
