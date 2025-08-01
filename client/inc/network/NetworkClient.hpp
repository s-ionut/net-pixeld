#pragma once

#include "Protocol.hpp"
#include "Logger.hpp"

#include <boost/asio.hpp>

#include <mutex>
#include <thread>
#include <queue>
#include <atomic>
#include <string>
#include <cstdint>

using json = nlohmann::json;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class NetworkClient
{
public:
    NetworkClient();
    ~NetworkClient();

    // Connect and start receive thread
    bool connect(const std::string &host, uint16_t port);

    void sendMessage(const Protocol::Message &msg);

    // Poll next message
    // Returns false if none available.
    bool pollMessage(Protocol::Message &out);

    void shutdown();

private:
    void recvLoop();

    asio::io_context m_ioContext;
    tcp::socket m_socket;
    asio::streambuf m_incoming;
    std::mutex m_sendMutex;

    std::atomic<bool> m_running{false};
    std::thread m_recvThread;
    std::mutex m_recvMutex;
    std::queue<Protocol::Message> m_recvQueue;
};
