#pragma once
#include <cstdint>

struct PlayerIdResponse {
    char uniqueId[33]; // 32 chars + null terminator
    uint32_t playerId;
};
