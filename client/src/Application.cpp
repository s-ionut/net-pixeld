#include "Application.hpp"

Application::Application(uint16_t width, uint16_t height, std::string windowName)
    : m_windowWidth(width),
      m_windowHeight(height),
      m_windowName(windowName),
      m_window(m_windowWidth, m_windowHeight, m_windowName)
{
    SetTargetFPS(60);

    m_net.connect("127.0.0.1", 6000);
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        Update();
        Draw();
    }
}

void Application::Update()
{
}

void Application::Draw()
{
    BeginDrawing();

    m_window.ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
