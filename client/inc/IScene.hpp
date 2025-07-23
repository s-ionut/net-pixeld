#pragma once

#include "GameContext.hpp"

class IScene
{
public:
    virtual ~IScene() = default;

    virtual void onEnter() = 0;

    virtual void onExit() = 0;

    virtual void Update() = 0;

    virtual void Draw() = 0;

    virtual bool handleMessage(const Protocol::Message &msg)
    {
        return false;
    }

    // virtual void handleInput(const InputEvent&) = 0;
protected:
    IScene(GameContext &ctx) : m_ctx(ctx) {}

    GameContext &m_ctx;
};