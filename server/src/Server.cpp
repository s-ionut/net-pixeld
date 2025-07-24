#include "Server.hpp"

Server::Server(uint16_t port, JsonDB &db)
    : m_ctx(), m_acceptor(m_ctx, tcp::endpoint(tcp::v4(), port)), m_resourceTimer(m_ctx), m_db(db)
{
    m_acceptor.set_option(tcp::acceptor::reuse_address(true));
    scheduleResourceTick();
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
    handshake.sequence = 1;
    handshake.payload = {{"clientId", id}};

    session->send(handshake);
    session->start();

    LOG_DEBUG("New session %d connected", int(id));
    doAccept();
}

void Server::scheduleResourceTick()
{
    m_resourceTimer.expires_after(std::chrono::seconds(1));
    m_resourceTimer.async_wait(
        [this](auto ec)
        { onResourceTick(ec); });
}

void Server::onResourceTick(boost::system::error_code ec)
{
    if (ec)
    {
        LOG_ERROR("Timer was canceled on shutdown");
        return;
    }

    for (auto &[clientId, session] : m_sessions)
    {
        if (!session->uuid().empty())
        {
            // test
            auto pool = m_db.getResources(session->uuid());
            pool.wood += 10;
            pool.food += 10;
            pool.iron += 10;

            Protocol::Message msg;
            msg.type = Protocol::MSG_RESOURCE_UPDATE;
            msg.sequence = ++m_lastSeqByType[clientId]["ResourceUpdate"];
            msg.payload = {
                {"uuid", session->uuid()},
                {"wood", pool.wood},
                {"food", pool.food},
                {"iron", pool.iron}};

            LOG_DEBUG("Sent %s type message", Protocol::MSG_RESOURCE_UPDATE);

            m_db.updateResources(session->uuid(), pool);
            session->send(msg);
        }
    }

    scheduleResourceTick();
}

void Server::sendToClient(uint8_t clientId, const Protocol::Message &msg)
{
    auto it = m_sessions.find(clientId);
    if (it != m_sessions.end())
    {
        it->second->send(msg);
    }
}

std::shared_ptr<Session> Server::getSession(const uint8_t clientId)
{
    return m_sessions[clientId];
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
        LOG_ERROR("Unhandled message type '%s' from client %d", type.c_str(), int(clientId));
    }
}
