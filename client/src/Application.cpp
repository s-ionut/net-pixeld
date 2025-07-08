#include "Application.hpp"

Application::Application(uint16_t width, uint16_t height, std::string windowName)
    : m_windowWidth(width),
      m_windowHeight(height),
      m_windowName(windowName),
      m_window(m_windowWidth, m_windowHeight, m_windowName)
{
    SetTargetFPS(60);

    m_net.connect("127.0.0.1", 6000);
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Draw();
    }
}

void Application::Update()
{
    Packet pkt;
    while (m_net.pollPacket(pkt))
    {
        if (pkt.header.type == MessageType::PositionUpdate)
        {
            // std::vector<EntitySate> ents;
            // PositionPayload pp = deserializePosition(pkt.payload, ents);

            // for (auto &entity : entities)
            // {
            //     m_world[e.id] = {e.x, e.y};
            // }
        }

        if (pkt.header.type == MessageType::AssignClientId)
        {
            AssignClientIdPayload clientIdPayload = deserializeClientId(pkt.payload);
            m_clientId = clientIdPayload.clientId;
            std::cout << "CLIENT GOT NEW PLAYER ID: " << static_cast<uint16_t>(m_clientId) << std::endl;
        }
    }
}

void Application::Draw()
{
    BeginDrawing();

    m_window.ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
