#include "Server.hpp"

int main()
{
    Server server(6000);
    server.registerHandler(Protocol::MSG_LOGIN_REQUEST,
                           [&server](uint8_t cId, const Protocol::Message &msg)
                           {
                               std::string id = msg.payload.at("id").get<std::string>();
                               std::string pass = msg.payload.at("password").get<std::string>();
                               LOG_DEBUG("Received id: %s and pass: %s", id.c_str(), pass.c_str());

                               Protocol::Message handshake;

                               handshake.type = Protocol::MSG_LOGIN_RESULT;
                               handshake.sequence = 1;
                               handshake.payload = {{"clientId", id}};

                               server.sendToClient(cId, handshake);
                           });

    server.run();
    return 0;
}
