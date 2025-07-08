#include "Deserialize.hpp"

AssignClientIdPayload deserializeClientId(const std::vector<uint8_t> &in)
{
    if (in.size() < sizeof(AssignClientIdPayload))
    {
        throw std::runtime_error("[deserializeClientId][ERROR]: payload too small");
    }

    AssignClientIdPayload out;
    std::memcpy(&out, in.data(), sizeof(out));

    return out;
}