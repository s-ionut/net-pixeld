#pragma once

#include "Packet.hpp"

#include <boost/asio.hpp>

#include <mutex>
#include <vector>
#include <cstdint>
#include <string>
#include <atomic>
#include <thread>
#include <queue>

class NetworkClient
{
public:
    NetworkClient();
    ~NetworkClient();

    bool connect(const std::string &host, uint16_t port);
    void sendPacket(const Packet &pkt);
    bool pollPacket(Packet &out);

    void shutdown();

private:
    void recvLoop();
    bool readExactly(void *buffer, size_t bytes);

    boost::asio::io_context      m_ioContext;
    boost::asio::ip::tcp::socket m_socket;

    // Sending
    std::mutex m_sendMutex;

    // Receiving
    std::atomic<bool>  m_running;
    std::thread        m_recvThread;
    std::mutex         m_recvMutex;
    std::queue<Packet> m_recvQueue;
};