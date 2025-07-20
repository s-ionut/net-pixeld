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
        // onMessage
        [this](uint8_t cid, const json &msg)
        {
            onClientMessage(cid, msg);
        },
        // onDisconnect
        [this](uint8_t cid)
        {
            onClientDisconnect(cid);
        });

    m_sessions[id] = session;

    // Send handshake
    json j = {
        {"type", "AssignClientId"},
        {"sequence", 0},
        {"payload", {{"clientId", id}}}};
    session->send(j);
    session->start();

    LOG_DEBUG("New session %d connected", int(id));
    doAccept();
}

void Server::onClientDisconnect(uint8_t clientId)
{
    m_sessions.erase(clientId);
    LOG_DEBUG("Session %d disconnected", int(clientId));
}

void Server::onClientMessage(uint8_t clientId, const json &msg)
{
    auto type = msg.value("type", std::string{});
    auto it = m_handlers.find(type);
    if (it != m_handlers.end())
    {
        it->second(clientId, msg);
    }
    else
    {
        LOG_ERROR("No handler for type '%s'", type);
    }
}
