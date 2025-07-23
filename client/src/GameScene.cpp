#include "GameScene.hpp"

void GameScene::onEnter()
{
    // TODO
}

void GameScene::onExit()
{
    // TODO
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
    DrawText(TextFormat("CLIENT:"), 200, 200, 12, DARKGRAY);
    DrawText(TextFormat("WOOD: %u", m_ctx.player.m_resources.wood), 200, 250, 12, DARKGRAY);
    DrawText(TextFormat("FOOD: %u", m_ctx.player.m_resources.food), 200, 300, 12, DARKGRAY);
    DrawText(TextFormat("IRON: %u", m_ctx.player.m_resources.iron), 200, 350, 12, DARKGRAY);
}

bool GameScene::handleMessage(const Protocol::Message &msg)
{
    if (msg.type == Protocol::MSG_RESOURCE_UPDATE)
    {
        uint8_t id = msg.payload.at("clientId").get<uint8_t>();
        if (id == m_ctx.clientId)
            m_ctx.player.UpdateResources((Resources){msg.payload.at("wood").get<uint64_t>(),
                                                     msg.payload.at("food").get<uint64_t>(),
                                                     msg.payload.at("iron").get<uint64_t>()});

        return true;
    }

    LOG_DEBUG("Unknown message type: %s", msg.type.c_str());
    return false;
}