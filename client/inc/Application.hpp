#pragma once

#include "raylib-cpp.hpp"

#include "NetworkClient.hpp"
#include "Payload.hpp"
#include "Deserialize.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

class Application
{
public:
    Application(uint16_t width, uint16_t height, std::string windowName);
    ~Application() = default;

    void Run();

private:
    void Update();
    void Draw();

private:
    std::string m_windowName;
    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    raylib::Window m_window;

    NetworkClient m_net;

    uint8_t m_clientId;

    std::unordered_map<uint8_t, raylib::Vector2> m_world;
};