#pragma once

#include "raylib-cpp.hpp"

#include "Protocol.hpp"
#include "Logger.hpp"

#include "network/NetworkClient.hpp"
#include "game/Player.hpp"
#include "scenes/SceneManager.hpp"
#include "scenes/GameScene.hpp"
#include "scenes/LoginScene.hpp"

#include <unordered_map>
#include <string>
#include <cstdint>

class Application
{
public:
    Application(uint16_t width, uint16_t height, std::string windowName);
    ~Application() = default;
    void Run();

private:
    void Update();
    void Draw();

    std::string m_windowName;
    uint16_t m_windowWidth, m_windowHeight;
    raylib::Window m_window;
    SceneManager m_scenes;

    NetworkClient m_net;
    Player m_player;
    GameContext m_ctx;
    std::deque<Protocol::Message> m_messageQueue;
};
