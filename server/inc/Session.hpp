#pragma once

#include "Packet.hpp"
#include "Payload.hpp"

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>
#include <boost/endian/conversion.hpp>

#include <cstdint>
#include <array>
#include <cstring>
#include <iostream>
#include <mutex>

using boost::asio::ip::tcp;

class Session
{
public:
    Session() = delete;
    ~Session();

    Session(tcp::socket &&skt, uint8_t id);

    void sendPacket(const Packet &pkt);

private:
    tcp::socket m_socket;
    std::mutex m_sendMutex;
    uint8_t m_clientId;
};