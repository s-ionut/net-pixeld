#include "Server.hpp"

int main()
{
    constexpr uint16_t listenPort = 6000;
    Server server(listenPort);
    server.run(); // blocking
    return 0;
}