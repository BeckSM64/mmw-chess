#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>
#include <thread>
#include <atomic>
#include "MMW.h"         // still safe to initialize MMW if you want
#include "ChessBoard.h"
#include "PlayerMove.h"

const char* PLAYER_MOVE_PUBLISH_TOPIC = "PLAYER_MOVE_PUBLISH";
const char* PLAYER_MOVE_SUBSCRIBE_TOPIC = "PLAYER_MOVE_SUBSCRIBE";
std::thread subscriberThread;
std::atomic<bool> running{true};
ChessBoard board(1920.f, 1080.f, 200.f);
uint32_t myPlayerId = 1;

void player_move_callback(void* message) {
    PlayerMove* remotePlayerMove = reinterpret_cast<PlayerMove*>(message);
    if (remotePlayerMove->playerId != myPlayerId) {
        board.applyMove(remotePlayerMove->fromX, remotePlayerMove->fromY, remotePlayerMove->toX, remotePlayerMove->toY);
    }
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
    if (argc >= 2) myPlayerId = static_cast<uint32_t>(atoi(argv[1]));
    std::cout << "Player ID: " << myPlayerId << std::endl;

    // optional: initialize middleware (harmless if not used yet)
    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    mmw_initialize("127.0.0.1", 5000);
    mmw_create_publisher(PLAYER_MOVE_PUBLISH_TOPIC);
    mmw_create_subscriber_raw(PLAYER_MOVE_SUBSCRIBE_TOPIC, player_move_callback);

    // Setup subscriber thread
    // subscriberThread = std::thread([]() {
    //     if (mmw_create_subscriber_raw(PLAYER_MOVE_SUBSCRIBE_TOPIC, player_move_callback) != MMW_OK) {
    //         std::cout << "Failed to create subscriber" << std::endl;
    //         return;
    //     }
    //     while (running) {
    //         std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //     }
    // });

    sf::RenderWindow window(sf::VideoMode(1280,720), "MMW Chess - Local Play");
    sf::View view(sf::FloatRect(0,0,1920,1080));
    window.setView(view);

    // ChessBoard board(1920.f, 1080.f, 200.f);
    board.initPieces();

    sf::Vector2i selected(-1,-1);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            } else if (e.type == sf::Event::Resized) {
                resizeView(window, view);
            } else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                // Convert pixel -> world coords (respecting view/viewport)
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                sf::Vector2i tile = board.getTileAtPixel(worldPos.x, worldPos.y);

                if (tile.x != -1) {
                    if (selected.x == -1) {
                        // select if there's a piece
                        if (board.hasPieceAt(tile.x, tile.y, myPlayerId))
                            selected = tile;
                    } else {
                        // move selected piece to clicked tile
                        board.applyMove(selected.x, selected.y, tile.x, tile.y);
                        
                        // Construct the player move to send to the server
                        PlayerMove playerMove;
                        playerMove.playerId = myPlayerId;
                        playerMove.fromX = selected.x;
                        playerMove.fromY = selected.y;
                        playerMove.toX = tile.x;
                        playerMove.toY = tile.y;

                        // Publish move to the server
                        mmw_publish_raw(PLAYER_MOVE_PUBLISH_TOPIC, &playerMove, sizeof(playerMove), MMW_BEST_EFFORT);

                        // Reset the selected tile
                        selected = sf::Vector2i(-1,-1);
                    }
                } else {
                    // clicked off-board: deselect
                    selected = sf::Vector2i(-1,-1);
                }
            }
        }

        window.clear(sf::Color(50,50,50));
        window.setView(view);

        board.draw(window);

        // draw highlight for selection (if any)
        if (selected.x != -1) {
            float ts = board.getTileSize();
            float bx = board.getBoardX();
            float by = board.getBoardY();
            sf::RectangleShape highlight(sf::Vector2f(ts, ts));
            highlight.setPosition(bx + selected.x * ts, by + selected.y * ts);
            highlight.setFillColor(sf::Color(0,0,0,0));
            highlight.setOutlineThickness(3.f);
            highlight.setOutlineColor(sf::Color(255,200,0));
            window.draw(highlight);
        }

        window.display();
    }

    // if (subscriberThread.joinable()) {
    //     subscriberThread.join();
    // }

    mmw_cleanup();
    return 0;
}
