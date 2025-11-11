#pragma once
#include <cstdint>

struct PlayerMove {
    uint32_t playerId;   // ID of the player making the move
    int fromX;           // 0–7
    int fromY;           // 0–7
    int toX;             // 0–7
    int toY;             // 0–7
    // Optional: promotion piece type, castling flags, etc.
};
