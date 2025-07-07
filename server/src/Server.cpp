#include "Server.hpp"
#include <iostream>

using boost::asio::ip::tcp;

Server::Server(uint16_t port)
    : m_ioContext(),
      m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port))
{
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
}

void Server::run()
{
    std::cout << "Server listening on port "
              << m_acceptor.local_endpoint().port() << "...\n";

    while (true)
    {
        tcp::socket clientSocket(m_ioContext);
        m_acceptor.accept(clientSocket);

        std::cout << "New connection from "
                  << clientSocket.remote_endpoint().address().to_string()
                  << ":" << clientSocket.remote_endpoint().port()
                  << "\n";

        clientSocket.close();
    }
}