#pragma once

#include "Session.hpp"

#include <cstdint>
#include <boost/asio.hpp>

class Server
{
public:
    explicit Server(uint16_t port);

    void run();

private:
    void onNewConnection(tcp::socket &&sock);

    uint8_t m_nextClientId;

    boost::asio::io_context m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;

    std::vector<std::unique_ptr<Session>> m_sessions;
};