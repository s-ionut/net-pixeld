#include "Server.hpp"

int main()
{
    Server server(6000);
    // server.registerHandler("MovePlayer",
    //                        [](uint8_t cid, const json &msg)
    //                        {
    //                            int x = msg["payload"]["x"];
    //                            int y = msg["payload"]["y"];
    //                            // …
    //                        });

    server.run();
    return 0;
}
