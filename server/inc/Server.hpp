#pragma once

#include "Session.hpp"
#include "Logger.hpp"

#include <boost/asio.hpp>
#include <unordered_map>
#include <functional>
#include <memory>

using HandlerFn = std::function<void(uint8_t, const Protocol::Message &)>;

class Server
{
public:
    explicit Server(uint16_t port);
    void run();

    // Register JSON‑type handlers by string
    void registerHandler(std::string type, HandlerFn handler);

private:
    void doAccept();
    void onAccept(boost::system::error_code ec, tcp::socket socket);
    void onClientDisconnect(uint8_t clientId);
    void onClientMessage(uint8_t clientId, const Protocol::Message &msg);

    boost::asio::io_context m_ctx;
    tcp::acceptor m_acceptor;
    uint8_t m_nextClientId{1};
    std::unordered_map<uint8_t, std::shared_ptr<Session>> m_sessions;
    std::unordered_map<std::string, HandlerFn> m_handlers;
};
