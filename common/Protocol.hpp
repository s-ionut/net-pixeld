#pragma once

#include "json.hpp"

using json = nlohmann::json;

namespace Protocol
{
    //-----------------------------------------------------------------------------
    // "type" strings as constexpr
    //-----------------------------------------------------------------------------
    inline constexpr char MSG_ASSIGN_CLIENT_ID[] = "AssignClientId";
    inline constexpr char MSG_RESOURCE_UPDATE[] = "ResourceUpdate";
    inline constexpr char MSG_LOGIN_REQUEST[] = "LoginRequest";
    inline constexpr char MSG_LOGIN_RESULT[] = "LoginResult";

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