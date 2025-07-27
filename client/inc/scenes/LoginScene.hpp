#pragma once

#include "scenes/IScene.hpp"
#include "gui/Textbox.hpp"

#include "raylib-cpp.hpp"

class LoginScene : public IScene
{
public:
    LoginScene(GameContext &ctx) : IScene(ctx) {}
    void onEnter() override;
    void onExit() override;
    void Update() override;
    void Draw() override;
    bool handleMessage(const Protocol::Message &msg) override;

private:
    std::string m_username;
    std::string m_pass;

    std::shared_ptr<CFont> m_font;

    TextBox m_usernameTextbox;
    TextBox m_passTextbox;
};
