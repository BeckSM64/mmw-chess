#include "GameClient.h"
#include "MMW.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <cstring>

const char* PLAYER_MOVE_PUBLISH_TOPIC = "PLAYER_MOVE_PUBLISH";
const char* PLAYER_MOVE_SUBSCRIBE_TOPIC = "PLAYER_MOVE_SUBSCRIBE";
const char* PLAYER_ID_REQUEST_TOPIC = "PLAYER_ID_REQUEST";
const char* PLAYER_ID_RESPONSE_TOPIC = "PLAYER_ID_RESPONSE";
const char* GAME_STATE_UPDATE_TOPIC = "GAME_STATE_UPDATE";

static GameClient* instance = nullptr;

GameClient::GameClient()
    : window(sf::VideoMode(1280,720), "MMW Chess"),
      view(sf::FloatRect(0,0,1920,1080)),
      board(1920.f,1080.f,200.f),
      myPlayerId(-1),
      isMyTurn(false),
      selected(-1,-1) {
    instance = this;
}

std::string GameClient::generateUuid() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    uint64_t p1 = dis(gen), p2 = dis(gen);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << p1
       << std::setw(16) << std::setfill('0') << p2;
    return ss.str();
}

void GameClient::initNetworking() {
    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    mmw_initialize("127.0.0.1", 5000);
    mmw_create_publisher(PLAYER_MOVE_PUBLISH_TOPIC);
    mmw_create_publisher(PLAYER_ID_REQUEST_TOPIC);
    mmw_create_subscriber_raw(PLAYER_MOVE_SUBSCRIBE_TOPIC, [](void* msg){ instance->handlePlayerMove(msg); });
    mmw_create_subscriber_raw(PLAYER_ID_RESPONSE_TOPIC, [](void* msg){ instance->handlePlayerIdResponse(msg); });
    mmw_create_subscriber_raw(GAME_STATE_UPDATE_TOPIC, [](void* msg){ instance->handleGameStateUpdate(msg); });
}

void GameClient::requestPlayerId() {
    uuid = generateUuid();
    std::cout << "Generated UUID: " << uuid << std::endl;
    mmw_publish(PLAYER_ID_REQUEST_TOPIC, uuid.c_str(), MMW_BEST_EFFORT);
}

void GameClient::handlePlayerMove(void* message) {
    PlayerMove* move = reinterpret_cast<PlayerMove*>(message);
    if (move->playerId != myPlayerId)
        board.applyMove(move->fromX, move->fromY, move->toX, move->toY);
}

void GameClient::handlePlayerIdResponse(void* message) {
    PlayerIdResponse* resp = reinterpret_cast<PlayerIdResponse*>(message);
    std::string recv(resp->uniqueId);
    if (recv == uuid) {
        myPlayerId = resp->playerId;
        std::cout << "Assigned Player ID: " << myPlayerId << std::endl;
    }
}

void GameClient::handleGameStateUpdate(void* message) {
    GameState* gs = reinterpret_cast<GameState*>(message);
    currentGameState = *gs;
    isMyTurn = (currentGameState.playerTurnId == myPlayerId);
}

void GameClient::resizeView() {
    const float targetW=1920.f, targetH=1080.f;
    float windowRatio=float(window.getSize().x)/window.getSize().y;
    float targetRatio=targetW/targetH;
    float sizeX=1.f, sizeY=1.f, posX=0.f, posY=0.f;
    if(windowRatio>targetRatio){ sizeX=targetRatio/windowRatio; posX=(1.f-sizeX)/2.f; }
    else if(windowRatio<targetRatio){ sizeY=windowRatio/targetRatio; posY=(1.f-sizeY)/2.f; }
    view.setViewport(sf::FloatRect(posX,posY,sizeX,sizeY));
}

void GameClient::run() {
    initNetworking();
    requestPlayerId();
    window.setView(view);
    board.initPieces();

    while(window.isOpen()) {
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type==sf::Event::Closed) window.close();
            else if(e.type==sf::Event::Resized) resizeView();
            else if(e.type==sf::Event::MouseButtonPressed && e.mouseButton.button==sf::Mouse::Left && currentGameState.isGameStarted && isMyTurn) {
                sf::Vector2i pixelPos=sf::Mouse::getPosition(window);
                sf::Vector2f worldPos=window.mapPixelToCoords(pixelPos);
                sf::Vector2i tile=board.getTileAtPixel(worldPos.x,worldPos.y);

                if(tile.x!=-1) {
                    if(selected.x==-1) {
                        if(board.hasPieceAt(tile.x,tile.y,myPlayerId))
                            selected=tile;
                    } else {
                        board.applyMove(selected.x,selected.y,tile.x,tile.y);
                        PlayerMove move{myPlayerId,selected.x,selected.y,tile.x,tile.y};
                        mmw_publish_raw(PLAYER_MOVE_PUBLISH_TOPIC,&move,sizeof(move),MMW_BEST_EFFORT);
                        isMyTurn=false;
                        selected={-1,-1};
                    }
                } else selected={-1,-1};
            }
        }

        window.clear(sf::Color(50,50,50));
        window.setView(view);
        board.draw(window);

        if(selected.x!=-1) {
            float ts=board.getTileSize(), bx=board.getBoardX(), by=board.getBoardY();
            sf::RectangleShape highlight({ts,ts});
            highlight.setPosition(bx+selected.x*ts,by+selected.y*ts);
            highlight.setFillColor(sf::Color(0,0,0,0));
            highlight.setOutlineThickness(3.f);
            highlight.setOutlineColor(sf::Color(255,200,0));
            window.draw(highlight);
        }
        window.display();
    }

    mmw_cleanup();
}
