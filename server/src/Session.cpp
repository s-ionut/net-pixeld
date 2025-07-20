#include "Session.hpp"

Session::Session(tcp::socket socket,
                 uint8_t clientId,
                 MessageCallback onMessage,
                 DisconnectCallback onDisconnect)
    : m_socket(std::move(socket)), m_strand(m_socket.get_executor()), m_clientId(clientId), m_onMessage(std::move(onMessage)), m_onDisconnect(std::move(onDisconnect))
{
}

Session::~Session()
{
    boost::system::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_both, ec);
    m_socket.close(ec);
}

void Session::start()
{
    doRead();
}

void Session::doRead()
{
    auto self = shared_from_this();
    asio::async_read_until(
        m_socket, m_incoming, '\n',
        asio::bind_executor(m_strand,
                            [this, self](boost::system::error_code ec, std::size_t /*bytes*/)
                            {
                                if (ec)
                                    return m_onDisconnect(m_clientId);

                                std::istream is(&m_incoming);
                                std::string line;
                                std::getline(is, line);
                                if (!line.empty() && line.back() == '\r')
                                    line.pop_back();

                                try
                                {
                                    auto message = Protocol::Message::parse(line);
                                    m_onMessage(m_clientId, message);
                                }
                                catch (std::exception &e)
                                {
                                    LOG_ERROR("JSON parse error: %s", e.what());
                                }

                                doRead();
                            }));
}

void Session::send(const Protocol::Message &msg)
{
    auto text = msg.serialize();

    auto self = shared_from_this();
    asio::post(m_strand,
               [this, self, text = std::move(text)]() mutable
               {
                   bool busy = !m_outgoing.empty();
                   m_outgoing.push_back(std::move(text));
                   if (!busy)
                       doWrite();
               });
}

void Session::doWrite()
{
    auto self = shared_from_this();
    asio::async_write(
        m_socket,
        asio::buffer(m_outgoing.front()),
        asio::bind_executor(m_strand,
                            [this, self](boost::system::error_code ec, std::size_t)
                            {
                                if (ec)
                                    return m_onDisconnect(m_clientId);
                                m_outgoing.pop_front();
                                if (!m_outgoing.empty())
                                    doWrite();
                            }));
}
