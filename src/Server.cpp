#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include "MMW.h"
#include "GameState.h"
#include "PlayerIdResponse.h"

struct PlayerMove {
    uint32_t playerId;
    int fromX, fromY;
    int toX, toY;
};

GameState gameState{false, 0, 0};

const char* PLAYER_MOVE_PUBLISH_TOPIC = "PLAYER_MOVE_PUBLISH";
const char* PLAYER_MOVE_SUBSCRIBE_TOPIC = "PLAYER_MOVE_SUBSCRIBE";
const char* PLAYER_ID_REQUEST_TOPIC = "PLAYER_ID_REQUEST";
const char* PLAYER_ID_RESPONSE_TOPIC = "PLAYER_ID_RESPONSE";
const char* GAME_STATE_UPDATE_TOPIC = "GAME_STATE_UPDATE";
const uint8_t MAX_PLAYERS = 2;

// 8x8 board: 0 = empty, 1 = white, 2 = black (can expand to piece IDs later)
uint8_t board[8][8] = {0};

void onClientMove(void* data) {
    PlayerMove* move = reinterpret_cast<PlayerMove*>(data);
    std::cout << "Player " << move->playerId 
              << " moves (" << move->fromX << "," << move->fromY << ") -> ("
              << move->toX << "," << move->toY << ")\n";

    // Apply move to internal board (no validation here)
    board[move->toY][move->toX] = board[move->fromY][move->fromX];
    board[move->fromY][move->fromX] = 0;

    // Forward move to all clients
    mmw_publish_raw(PLAYER_MOVE_SUBSCRIBE_TOPIC, move, sizeof(PlayerMove), MMW_BEST_EFFORT);
    std::cout << "Published Message" << std::endl;
}

void onPlayerIdRequest(const char* uuid) {
    PlayerIdResponse playerIdResponse;
    playerIdResponse.playerId = gameState.connectedPlayers;
    strncpy(playerIdResponse.uniqueId, uuid, 32);
    playerIdResponse.uniqueId[32] = '\0';
    mmw_publish_raw(PLAYER_ID_RESPONSE_TOPIC, &playerIdResponse, sizeof(playerIdResponse), MMW_BEST_EFFORT);
    gameState.connectedPlayers++;
    std::cout << "Current number of connected players: " << std::to_string(gameState.connectedPlayers) << std::endl;
    if (gameState.connectedPlayers == MAX_PLAYERS) {
        gameState.isGameStarted = true;
        gameState.playerTurnId = 0;
        mmw_publish_raw(GAME_STATE_UPDATE_TOPIC, &gameState, sizeof(gameState), MMW_BEST_EFFORT);
    }
}

int main() {
    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    if (mmw_initialize("127.0.0.1", 5000) != MMW_OK) {
        std::cerr << "Failed to initialize MMW\n";
        return 1;
    }

    if (mmw_create_publisher(PLAYER_MOVE_SUBSCRIBE_TOPIC) != MMW_OK) {
        std::cerr << "Failed to create publisher\n";
        return 1;
    }

    if (mmw_create_publisher(PLAYER_ID_RESPONSE_TOPIC) != MMW_OK) {
        std::cerr << "Failed to create publisher\n";
        return 1;
    }

    if (mmw_create_publisher(GAME_STATE_UPDATE_TOPIC) != MMW_OK) {
        std::cerr << "Failed to create subscriber\n";
        return 1;
    }

    if (mmw_create_subscriber_raw(PLAYER_MOVE_PUBLISH_TOPIC, onClientMove) != MMW_OK) {
        std::cerr << "Failed to create subscriber\n";
        return 1;
    }

    if (mmw_create_subscriber(PLAYER_ID_REQUEST_TOPIC, onPlayerIdRequest) != MMW_OK) {
        std::cerr << "Failed to create subscriber\n";
        return 1;
    }

    std::cout << "Server running..." << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    mmw_cleanup();
    return 0;
}
