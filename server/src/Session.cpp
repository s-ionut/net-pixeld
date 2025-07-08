#include "Session.hpp"

Session::Session(tcp::socket &&skt, uint8_t id)
    : m_socket(std::move(skt)),
      m_clientId(id)
{
    std::cout << "Created new session ID: " << static_cast<uint16_t>(m_clientId) << std::endl;
}

Session::~Session()
{
    std::cout << "Deleted session: " << static_cast<uint16_t>(m_clientId) << std::endl;
    boost::system::error_code errCode;
    m_socket.shutdown(tcp::socket::shutdown_both, errCode);
    m_socket.close(errCode);
}

void Session::sendPacket(const Packet &pkt)
{
    // Prep. header
    PacketHeader hdr = pkt.header;
    boost::endian::native_to_big_inplace(hdr.sequence);
    boost::endian::native_to_big_inplace(hdr.payloadLength);

    // length prefix (header + payload)
    uint32_t total = static_cast<uint32_t>(sizeof(PacketHeader) + pkt.payload.size());
    boost::endian::native_to_big_inplace(total);
    std::array<uint8_t, sizeof(uint32_t)> lenBuf;

    std::memcpy(lenBuf.data(), &total, sizeof(uint32_t));

    // [len][hdr][payload]
    std::array<boost::asio::const_buffer, 3> bufs = {
        boost::asio::buffer(lenBuf),
        boost::asio::buffer(&hdr, sizeof(hdr)),
        boost::asio::buffer(pkt.payload)};

    // Send
    std::lock_guard<std::mutex> lock(m_sendMutex);
    try
    {
        boost::asio::write(m_socket, bufs);
    }
    catch (std::exception &e)
    {
        std::cerr << "[Session::sendPacket][Error]: " << e.what() << "\n";
        // handle disconnect somehow :(
        m_socket.close();
    }
}