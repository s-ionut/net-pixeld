#pragma once

#include "Payload.hpp"

#include <vector>
#include <cstring>
#include <stdexcept>

AssignClientIdPayload deserializeClientId(const std::vector<uint8_t> &in);