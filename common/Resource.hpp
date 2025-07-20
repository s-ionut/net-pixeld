#pragma once

#include "json.hpp"

using json = nlohmann::json;

struct Resources
{
    uint64_t wood = 0;
    uint64_t food = 0;
    uint64_t iron = 0;
};