#include "NetworkClient.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>

NetworkClient::NetworkClient()
    : m_socket(m_ioContext)
{
}

NetworkClient::~NetworkClient()
{
    shutdown();
    if (m_recvThread.joinable())
        m_recvThread.join();
}

void NetworkClient::shutdown()
{
    if (!m_running)
        return;
    m_running = false;

    boost::system::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_both, ec);
    m_socket.close(ec);
}

bool NetworkClient::connect(const std::string &host, uint16_t port)
{
    try
    {
        tcp::resolver resolver(m_ioContext);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(m_socket, endpoints);
        m_running = true;
        m_recvThread = std::thread(&NetworkClient::recvLoop, this);
        return true;
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s", e.what());
        return false;
    }
}

void NetworkClient::recvLoop()
{
    while (m_running)
    {
        boost::system::error_code ec;
        std::size_t n = boost::asio::read_until(m_socket, m_incoming, '\n', ec);
        if (ec)
            break;

        std::istream is(&m_incoming);
        std::string line;
        std::getline(is, line);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        try
        {
            json msg = json::parse(line);
            std::lock_guard lk(m_recvMutex);
            m_recvQueue.push(std::move(msg));
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("JSON parse error: %s", e.what());
        }
    }
    m_running = false;
}

void NetworkClient::sendMessage(const json &msg)
{
    std::string out = msg.dump();
    out.push_back('\n');
    std::lock_guard lk(m_sendMutex);
    boost::asio::write(m_socket, boost::asio::buffer(out));
}

bool NetworkClient::pollMessage(json &out)
{
    std::lock_guard lk(m_recvMutex);
    if (m_recvQueue.empty())
        return false;
    out = std::move(m_recvQueue.front());
    m_recvQueue.pop();
    return true;
}