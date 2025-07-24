#include "LoginScene.hpp"

void LoginScene::onEnter()
{
    // TODO
}

void LoginScene::onExit()
{
    // TODO
}

void LoginScene::Update()
{
    static bool test = true;

    if (test)
    {
        Protocol::Message msg;
        msg.type = Protocol::MSG_LOGIN_REQUEST;
        msg.sequence = 1;
        msg.payload = {
            {"id", "test1"},
            {"password", "test2"}};

        LOG_DEBUG("Sent %s type message", Protocol::MSG_LOGIN_REQUEST);

        m_ctx.net.sendMessage(msg);

        test = false;
    }
}

void LoginScene::Draw()
{
    DrawText(TextFormat("CLIENT LOGIN:"), 200, 200, 12, DARKGRAY);
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