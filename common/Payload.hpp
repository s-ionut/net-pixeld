#pragma once

#include <cstdint>

struct AssignClientIdPayload
{
    uint8_t clientId;
};

struct PositionPayload
{
    uint8_t id;
    float x, y;
};