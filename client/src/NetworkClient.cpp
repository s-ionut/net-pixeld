#include "NetworkClient.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/endian/conversion.hpp> // for big-endian helpers

#include <iostream>
#include <array>
#include <mutex>

using boost::asio::ip::tcp;

NetworkClient::NetworkClient()
    : m_ioContext(),
      m_socket(m_ioContext),
      m_running(false)
{
}

NetworkClient::~NetworkClient()
{
    shutdown();
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
    catch (std::exception &e)
    {
        std::cerr << "[NetworkClient::connect][ERROR]: " << e.what() << "\n";
        return false;
    }
}

void NetworkClient::recvLoop()
{
    while (m_running)
    {
        // 1) Read length prefix
        uint32_t beLen;
        if (!readExactly(&beLen, sizeof(beLen)))
            break;
        uint32_t packetLen = boost::endian::big_to_native(beLen);

        // 2) Read header
        PacketHeader hdr;
        if (!readExactly(&hdr, sizeof(hdr)))
            break;
        // convert fields back to host order
        hdr.sequence = boost::endian::big_to_native(hdr.sequence);
        hdr.payloadLength = boost::endian::big_to_native(hdr.payloadLength);

        // 3) Read payload
        std::vector<uint8_t> payload(hdr.payloadLength);
        if (hdr.payloadLength > 0)
        {
            if (!readExactly(payload.data(), hdr.payloadLength))
                break;
        }

        // 4) Queue it up
        Packet pkt{hdr, std::move(payload)};
        {
            std::lock_guard<std::mutex> lk(m_recvMutex);
            m_recvQueue.push(std::move(pkt));
        }
    }
}

bool NetworkClient::readExactly(void *buffer, size_t bytes)
{
    try
    {
        boost::asio::read(m_socket, boost::asio::buffer(buffer, bytes));
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool NetworkClient::pollPacket(Packet &out)
{
    std::lock_guard<std::mutex> lk(m_recvMutex);
    if (m_recvQueue.empty())
        return false;
    out = std::move(m_recvQueue.front());
    m_recvQueue.pop();
    return true;
}

void NetworkClient::sendPacket(const Packet &pkt)
{
    // Prep. PacketHeader
    PacketHeader hdr = pkt.header;
    boost::endian::native_to_big_inplace(hdr.sequence);
    boost::endian::native_to_big_inplace(hdr.payloadLength);

    // (length of header + payload)
    uint32_t total = static_cast<uint32_t>(sizeof(PacketHeader) + pkt.payload.size());
    boost::endian::native_to_big_inplace(total);

    std::array<uint8_t, sizeof(total)> lenBuf;
    std::memcpy(lenBuf.data(), &total, sizeof(total));

    // [len][hdr][payload]
    std::array<boost::asio::const_buffer, 3> bufs = {
        boost::asio::buffer(lenBuf),
        boost::asio::buffer(&hdr, sizeof(hdr)),
        boost::asio::buffer(pkt.payload)};

    // Send
    std::lock_guard<std::mutex> lock(m_sendMutex);
    boost::asio::write(m_socket, bufs);
}

void NetworkClient::shutdown()
{
    boost::system::error_code errCode;
    m_socket.shutdown(tcp::socket::shutdown_both, errCode);
    m_socket.close(errCode);
}