#include "Application.hpp"

Application::Application(uint16_t width, uint16_t height, std::string windowName)
    : m_windowWidth(width),
      m_windowHeight(height),
      m_windowName(std::move(windowName)),
      m_window(m_windowWidth, m_windowHeight, m_windowName),
      m_ctx{0, m_player, m_net}
{
    SetTargetFPS(60);

    m_scenes.registerScene<LoginScene>(SceneID::Login, m_ctx);
    m_scenes.registerScene<GameScene>(SceneID::Game, m_ctx);
    m_scenes.changeScene(SceneID::Login);

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
        auto &lastSeq = m_ctx.lastSeqByType[msg.type];

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
            m_ctx.clientId = msg.payload.at("clientId").get<uint8_t>();
            LOG_DEBUG("Assigned client ID: %u", m_ctx.clientId);
            continue;
        }

        auto *scene = m_scenes.current();
        bool handled = (scene && scene->handleMessage(msg));
        if (!handled)
        {
            // drop it for now
        }
    }

    if (!m_ctx.uuid.empty())
    {
        m_scenes.changeScene(SceneID::Game);
    }

    m_scenes.Update();
}

void Application::Draw()
{
    BeginDrawing();

    m_window.ClearBackground(RAYWHITE);
    m_scenes.Draw();

    EndDrawing();
}
