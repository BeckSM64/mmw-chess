#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <string>
#include "ChessBoard.h"
#include "GameState.h"
#include "PlayerMove.h"
#include "PlayerIdResponse.h"

class GameClient {
public:
    GameClient();
    void run();

private:
    void initNetworking();
    void requestPlayerId();
    void handlePlayerMove(void* message);
    void handlePlayerIdResponse(void* message);
    void handleGameStateUpdate(void* message);
    void resizeView();
    std::string generateUuid();

    sf::RenderWindow window;
    sf::View view;
    ChessBoard board;
    std::string uuid;
    uint32_t myPlayerId;
    bool isMyTurn;
    GameState currentGameState;
    sf::Vector2i selected;
};
