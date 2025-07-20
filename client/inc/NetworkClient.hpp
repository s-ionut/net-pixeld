#pragma once

#include "Logger.hpp"

#include <boost/asio.hpp>
#include <json.hpp>

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

    // Send message (type/payload/sequence).
    void sendMessage(const json &msg);

    // Poll next message
    // Returns false if none available.
    bool pollMessage(json &out);

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
    std::queue<json> m_recvQueue;
};
