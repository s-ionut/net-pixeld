#pragma once

#include "json.hpp"

using json = nlohmann::json;

struct Resources
{
    uint64_t wood = 0;
    uint64_t food = 0;
    uint64_t iron = 0;
};

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------
// inline void to_json(json &j, const Resources &r)
// {
//     j = json{{"wood", r.wood}, {"food", r.food}, {"iron", r.iron}};
// }

// inline void from_json(const json &j, Resources &r)
// {
//     r.wood = j.at("wood").get<uint64_t>();
//     r.food = j.at("food").get<uint64_t>();
//     r.iron = j.at("iron").get<uint64_t>();
// }