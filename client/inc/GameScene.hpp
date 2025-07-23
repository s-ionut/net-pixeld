#pragma once

#include "IScene.hpp"

#include "raylib-cpp.hpp"

class GameScene : public IScene
{
public:
    GameScene(GameContext &ctx) : IScene(ctx) {}
    void onEnter() override;
    void onExit() override;
    void Update() override;
    void Draw() override;
    bool handleMessage(const Protocol::Message &msg) override;
};
