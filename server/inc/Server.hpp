#pragma once

#include <cstdint>
#include <boost/asio.hpp>

class Server
{
public:
    explicit Server(uint16_t port);

    void run();

private:
    boost::asio::io_context        m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
};