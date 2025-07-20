#include "Application.hpp"

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
        auto &lastSeq = m_lastSeqByType[msg.type];
        if (msg.sequence <= lastSeq)
            continue;
        if (msg.sequence != lastSeq + 1)
        {
            LOG_ERROR("Out of order %s: expected %u got %u",
                      msg.type.c_str(), lastSeq + 1, msg.sequence);
            continue;
        }
        lastSeq = msg.sequence;

        if (msg.type == Protocol::MSG_ASSIGN_CLIENT_ID)
        {
            m_clientId = msg.payload.at("clientId").get<uint8_t>();
            LOG_DEBUG("Assigned client ID: %u", m_clientId);
        }
        else if (msg.type == Protocol::MSG_RESOURCE_UPDATE)
        {
            uint8_t id = msg.payload.at("clientId").get<uint8_t>();
            if (id == m_clientId)
                m_player.UpdateResources((Resources){msg.payload.at("wood").get<uint64_t>(),
                                                     msg.payload.at("food").get<uint64_t>(),
                                                     msg.payload.at("iron").get<uint64_t>()});
        }
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

    DrawText(TextFormat("CLIENT:"), 200, 200, 12, DARKGRAY);
    DrawText(TextFormat("WOOD: %u", m_player.m_resources.wood), 200, 250, 12, DARKGRAY);
    DrawText(TextFormat("FOOD: %u", m_player.m_resources.food), 200, 300, 12, DARKGRAY);
    DrawText(TextFormat("IRON: %u", m_player.m_resources.iron), 200, 350, 12, DARKGRAY);

    EndDrawing();
}
