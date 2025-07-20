#pragma once

#include "json.hpp"

#include <string>
#include <vector>

using json = nlohmann::json;
namespace Protocol
{
    //-----------------------------------------------------------------------------
    // 1) All your "type" strings as constexpr
    //-----------------------------------------------------------------------------
    inline constexpr char MSG_ASSIGN_CLIENT_ID[] = "AssignClientId";
    inline constexpr char MSG_POSITION_UPDATE[] = "PositionUpdate";
    // ? add more as you grow ?

    //-----------------------------------------------------------------------------
    // 2) A small enum for switch?style dispatch (optional)
    //-----------------------------------------------------------------------------
    enum class MessageType
    {
        AssignClientId,
        PositionUpdate,
        Unknown
    };

    inline MessageType messageTypeFromString(std::string_view s)
    {
        if (s == MSG_ASSIGN_CLIENT_ID)
            return MessageType::AssignClientId;
        if (s == MSG_POSITION_UPDATE)
            return MessageType::PositionUpdate;
        return MessageType::Unknown;
    }

    //-----------------------------------------------------------------------------
    // 3) A common wrapper for your JSON messages
    //-----------------------------------------------------------------------------
    struct Message
    {
        std::string type; // e.g. "AssignClientId"
        uint32_t sequence;
        json payload;

        // Build a JSON blob ready to send
        std::string serialize() const
        {
            json j = {
                {"type", type},
                {"sequence", sequence},
                {"payload", payload}};
            return j.dump() + "\n";
        }

        // Parse from a raw JSON string (throws on parse error)
        static Message parse(std::string_view raw)
        {
            auto j = json::parse(raw);
            return Message{
                j.at("type").get<std::string>(),
                j.at("sequence").get<uint32_t>(),
                j.at("payload")};
        }
    };
} // Protocol