#pragma once

#include "json.hpp"

#include <string>
#include <vector>

using json = nlohmann::json;

namespace Protocol
{
    //-----------------------------------------------------------------------------
    // "type" strings as constexpr
    //-----------------------------------------------------------------------------
    inline constexpr char MSG_ASSIGN_CLIENT_ID[] = "AssignClientId";
    inline constexpr char MSG_RESOURCE_UPDATE[] = "ResourceUpdate";

    //-----------------------------------------------------------------------------
    // Enum for switch?style dispatch
    //-----------------------------------------------------------------------------
    enum class MessageType
    {
        AssignClientId,
        ResourceUpdate,
        Unknown
    };

    inline MessageType messageTypeFromString(std::string_view s)
    {
        if (s == MSG_ASSIGN_CLIENT_ID)
            return MessageType::AssignClientId;
        if (s == MSG_RESOURCE_UPDATE)
            return MessageType::ResourceUpdate;
        return MessageType::Unknown;
    }

    //-----------------------------------------------------------------------------
    // Common wrapper for JSON messages
    //-----------------------------------------------------------------------------
    struct Message
    {
        std::string type;
        uint32_t sequence;
        json payload;

        std::string serialize() const
        {
            json j = {
                {"type", type},
                {"sequence", sequence},
                {"payload", payload}};
            return j.dump() + "\n";
        }

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