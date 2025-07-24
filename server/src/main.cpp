#include "Server.hpp"

int main()
{
    JsonDB db("accounts.json");
    Server server(6000, db);

    server.registerHandler(Protocol::MSG_LOGIN_REQUEST,
                           [&server, &db](uint8_t clientId, const Protocol::Message &msg)
                           {
                               auto &p = msg.payload;
                               std::string user = p.at("id").get<std::string>();
                               std::string pass = p.at("password").get<std::string>();
                               LOG_DEBUG("Received id: %s and pass: %s", user.c_str(), pass.c_str());

                               std::string uuid;
                               if (db.userExists(user))
                               {
                                   if (!db.verifyUser(user, pass))
                                   {
                                       LOG_ERROR("Invalid password for %s", user.c_str());
                                       return;
                                   }
                                   uuid = db.getUser(user, pass);
                               }
                               else
                               {
                                   uuid = db.addUser(user, pass);
                               }

                               server.getSession(clientId)->setUUID(uuid);

                               Protocol::Message reply;
                               reply.type = Protocol::MSG_LOGIN_RESULT;
                               reply.sequence = 1;
                               reply.payload = {
                                   {"uuid", uuid}};
                               server.sendToClient(clientId, reply);
                           });

    server.run();
    return 0;
}
