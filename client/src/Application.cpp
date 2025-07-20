#include "Application.hpp"

using json = nlohmann::json;

Application::Application(uint16_t width, uint16_t height, std::string windowName)
    : m_windowWidth(width), m_windowHeight(height), m_windowName(std::move(windowName)), m_window(m_windowWidth, m_windowHeight, m_windowName)
{
    SetTargetFPS(60);
    if (!m_net.connect("127.0.0.1", 6000))
        LOG_ERROR("Failed to connect to server");
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Draw();
    }

    m_net.shutdown();
}

void Application::Update()
{
    json msg;
    while (m_net.pollMessage(msg))
    {
        auto type = msg.value("type", std::string{});
        if (type == "AssignClientId")
        {
            m_clientId = msg["payload"]["clientId"].get<uint8_t>();
            LOG_DEBUG("Assigned client ID: %d", int(m_clientId));
        }
        else if (type == "PositionUpdate")
        {
            // expecting payload.entities = [ {id,x,y}, ... ]
            //     for (auto &ent : msg["payload"]["entities"])
            //     {
            //         uint8_t id = ent["id"].get<uint8_t>();
            //         float x = ent["x"].get<float>();
            //         float y = ent["y"].get<float>();
            //         m_world[id] = {x, y};
            //     }
        }
        else
        {
            LOG_ERROR("Unknown message type: %s", type);
        }
    }
}

void Application::Draw()
{
    BeginDrawing();
    m_window.ClearBackground(RAYWHITE);

    DrawText(TextFormat("CLIENT"), 200, 200, 12, DARKGRAY);

    EndDrawing();
}
