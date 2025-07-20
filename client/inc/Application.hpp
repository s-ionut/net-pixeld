#pragma once

#include "raylib-cpp.hpp"

#include "Protocol.hpp"
#include "Logger.hpp"

#include "NetworkClient.hpp"

#include <json.hpp>

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
    NetworkClient m_net;
    uint8_t m_clientId = 0;
    std::unordered_map<uint8_t, raylib::Vector2> m_world;
};
