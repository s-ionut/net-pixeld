#pragma once

#include "Protocol.hpp"
#include "Logger.hpp"

#include <boost/asio.hpp>

#include <deque>
#include <memory>
#include <functional>
#include <cstdint>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    using MessageCallback = std::function<void(uint8_t /*clientId*/, const Protocol::Message & /*msg*/)>;
    using DisconnectCallback = std::function<void(uint8_t)>;

    Session(tcp::socket socket,
            uint8_t clientId,
            MessageCallback onMessage,
            DisconnectCallback onDisconnect);

    ~Session();
    void start();
    void send(const Protocol::Message &msg);

    uint8_t id() const { return m_clientId; }

private:
    void doRead();
    void doWrite();

    tcp::socket m_socket;
    asio::strand<tcp::socket::executor_type> m_strand;
    asio::streambuf m_incoming;
    std::deque<std::string> m_outgoing;
    uint8_t m_clientId;
    MessageCallback m_onMessage;
    DisconnectCallback m_onDisconnect;
};
