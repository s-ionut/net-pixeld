#pragma once

#include "Resource.hpp"

#include <cstdint>

class Player
{
public:
    Player();
    ~Player();

    void UpdateResources(Resources res);

public:
    Resources m_resources;
};