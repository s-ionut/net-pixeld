#pragma once

#include "raylib.h"

#include <functional>
#include <string>

class Button
{
public:
    // Constructor
    Button(Rectangle bounds, const std::string &text = "", Font *font = nullptr);

    // Optional configuration
    void SetTextures(Texture2D *normal, Texture2D *hover = nullptr, Texture2D *pressed = nullptr);
    void SetSound(Sound *clickSound);
    void SetCallbacks(std::function<void()> onClick, std::function<void()> onHover = nullptr, std::function<void()> onRelease = nullptr);

    // Update & draw
    void Update();
    void Draw();

private:
    Rectangle bounds;
    std::string text;
    Font *font;

    Texture2D *texNormal = nullptr;
    Texture2D *texHover = nullptr;
    Texture2D *texPressed = nullptr;
    Sound *clickSound = nullptr;

    std::function<void()> onClickCallback = nullptr;
    std::function<void()> onHoverCallback = nullptr;
    std::function<void()> onReleaseCallback = nullptr;

    enum State
    {
        NORMAL,
        HOVERED,
        PRESSED
    } state = NORMAL;
    bool wasPressed = false;
};
