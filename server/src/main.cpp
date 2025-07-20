#include "Server.hpp"

int main()
{
    Server server(6000);
    // server.registerHandler("ResourceUpdate",
    //                        [](uint8_t cId, const json &msg)
    //                        {
    //                            int x = msg["payload"]["x"];
    //                            int y = msg["payload"]["y"];
    //                            // …
    //                        });

    server.run();
    return 0;
}
