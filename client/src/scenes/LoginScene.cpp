#include "scenes/LoginScene.hpp"

void LoginScene::onEnter()
{
    m_usernameTextbox.SetCenter({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f - 50});
    m_usernameTextbox.SetDimensions({200.0f, 40.0f});
    m_usernameTextbox.SetFocusAnimation(true);

    m_passTextbox.SetCenter({GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f + 50});
    m_passTextbox.SetDimensions({200.0f, 40.0f});
    m_passTextbox.SetFocusAnimation(true);
}

void LoginScene::onExit()
{
    // TODO
}

void LoginScene::Update()
{
    static bool test = true;

    m_usernameTextbox.Update();
    m_passTextbox.Update();

    m_username = m_usernameTextbox.GetText();
    m_pass = m_passTextbox.GetText();

    if (!m_username.empty() && !m_pass.empty())
    {
        Protocol::Message msg;
        msg.type = Protocol::MSG_LOGIN_REQUEST;
        msg.sequence = 1;
        msg.payload = {
            {"id", m_username},
            {"password", m_pass}};

        LOG_DEBUG("Sent %s type message", Protocol::MSG_LOGIN_REQUEST);

        m_ctx.net.sendMessage(msg);

        test = false;
    }
}

void LoginScene::Draw()
{
    DrawText(TextFormat("LOGIN"), GetScreenWidth() / 2 - 50, 10, 30, DARKGRAY);
    m_usernameTextbox.Draw();
    m_passTextbox.Draw();
}

bool LoginScene::handleMessage(const Protocol::Message &msg)
{
    if (msg.type == Protocol::MSG_LOGIN_RESULT)
    {
        m_ctx.uuid = msg.payload.at("uuid").get<std::string>();
        LOG_DEBUG("LoginScene got uuid=%s", m_ctx.uuid.c_str());
        return true;
    }

    LOG_DEBUG("Unknown message type: %s", msg.type.c_str());
    return false;
}