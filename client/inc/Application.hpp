#pragma once

#include "raylib-cpp.hpp"

#include "NetworkClient.hpp"

#include <string>

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
};