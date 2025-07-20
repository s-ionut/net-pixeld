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
    Protocol::Message msg;
    while (m_net.pollMessage(msg))
    {
        if (msg.type == Protocol::MSG_ASSIGN_CLIENT_ID)
        {
            m_clientId = msg.payload.at("clientId").get<uint8_t>();
            LOG_DEBUG("Assigned client ID: %u", m_clientId);
        }
        // else if (msg.type == MSG_POSITION_UPDATE)
        // {
        //     uint8_t id = msg.payload.at("clientId").get<uint8_t>();
        //     float x = msg.payload.at("x").get<float>();
        //     float y = msg.payload.at("y").get<float>();
        //     m_world[id] = {x, y};
        // }
        else
        {
            LOG_DEBUG("Unknown message type: %s", msg.type.c_str());
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
