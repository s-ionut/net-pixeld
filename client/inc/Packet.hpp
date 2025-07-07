#pragma once

#include <vector>
#include <cstdint>

enum class MessageType : uint8_t
{
    Input = 0,
    CustomEvent = 1,
    // TODO: add more types
};

struct PacketHeader
{
    uint8_t version;
    MessageType type;
    uint16_t sequence;
    uint32_t payloadLength;
};

struct Packet
{
    PacketHeader header;
    std::vector<uint8_t> payload;
};