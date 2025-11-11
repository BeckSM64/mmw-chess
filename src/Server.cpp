#include <iostream>
#include <thread>
#include <chrono>
#include "MMW.h"

struct PlayerMove {
    uint32_t playerId;
    int fromX, fromY;
    int toX, toY;
};

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
    mmw_publish_raw("state", move, sizeof(PlayerMove), MMW_BEST_EFFORT);
}

int main() {
    mmw_set_log_level(MMW_LOG_LEVEL_OFF);
    if (mmw_initialize("127.0.0.1", 5000) != MMW_OK) {
        std::cerr << "Failed to initialize MMW\n";
        return 1;
    }

    if (mmw_create_subscriber_raw("input", onClientMove) != MMW_OK) {
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
