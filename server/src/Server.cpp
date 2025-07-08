#include "Server.hpp"
#include <iostream>

using boost::asio::ip::tcp;

Server::Server(uint16_t port)
    : m_ioContext(),
      m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port)),
      m_nextClientId(0)
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

        onNewConnection(std::move(clientSocket));

        // std::cout << "New connection from "
        //           << clientSocket.remote_endpoint().address().to_string()
        //           << ":" << clientSocket.remote_endpoint().port()
        //           << "\n";

        // clientSocket.close();
    }
}

void Server::onNewConnection(tcp::socket &&skt)
{
    uint8_t currClientId = m_nextClientId++;
    auto sess = std::make_unique<Session>(std::move(skt), currClientId);
    m_sessions.push_back(std::move(sess));

    // build packet
    AssignClientIdPayload clientIdPayload{currClientId};
    Packet pkt;

    pkt.header.version = 1;
    pkt.header.type = MessageType::AssignClientId;
    pkt.header.sequence = 0;
    pkt.header.payloadLength = sizeof(clientIdPayload);
    pkt.payload.resize(sizeof(clientIdPayload));

    std::memcpy(pkt.payload.data(), &clientIdPayload, sizeof(clientIdPayload));

    m_sessions.back()->sendPacket(pkt);

    std::cout << "New session ID: " << static_cast<uint16_t>(currClientId) << " registered!\n";
}