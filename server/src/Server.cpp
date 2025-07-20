#include "Server.hpp"

Server::Server(uint16_t port)
    : m_ctx(), m_acceptor(m_ctx, tcp::endpoint(tcp::v4(), port))
{
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
}

void Server::run()
{
    doAccept();
    m_ctx.run();
}

void Server::registerHandler(std::string type, HandlerFn handler)
{
    m_handlers[std::move(type)] = std::move(handler);
}

void Server::doAccept()
{
    m_acceptor.async_accept(
        [this](auto ec, tcp::socket sock)
        {
            onAccept(ec, std::move(sock));
        });
}

void Server::onAccept(boost::system::error_code ec, tcp::socket sock)
{
    if (ec)
    {
        LOG_ERROR("Accept error: %s", ec.message());
        return doAccept();
    }

    uint8_t id = m_nextClientId++;
    auto session = std::make_shared<Session>(
        std::move(sock),
        id,
        [this](uint8_t cId, const Protocol::Message &msg)
        {
            onClientMessage(cId, msg);
        },
        [this](uint8_t cId)
        {
            onClientDisconnect(cId);
        });

    m_sessions[id] = session;

    // Send handshake
    Protocol::Message handshake;

    handshake.type = Protocol::MSG_ASSIGN_CLIENT_ID;
    handshake.sequence = 0;
    handshake.payload = {{"clientId", id}};

    session->send(handshake);
    session->start();

    LOG_DEBUG("New session %d connected", int(id));
    doAccept();
}

void Server::onClientDisconnect(uint8_t clientId)
{
    m_sessions.erase(clientId);
    LOG_DEBUG("Session %d disconnected", int(clientId));
}

void Server::onClientMessage(uint8_t clientId, const Protocol::Message &msg)
{
    const auto &type = msg.type;

    auto it = m_handlers.find(type);
    if (it != m_handlers.end())
    {
        it->second(clientId, msg);
    }
    else
    {
        LOG_ERROR("Unhandled message type '%s' from client %d", type, int(clientId));
    }
}
