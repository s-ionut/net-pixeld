#pragma once

#include "game/Player.hpp"
#include "network/NetworkClient.hpp"

struct GameContext
{
    uint32_t clientId;
    Player &player;
    NetworkClient &net;
    std::unordered_map<std::string, uint32_t> lastSeqByType;
    std::string uuid;
};
