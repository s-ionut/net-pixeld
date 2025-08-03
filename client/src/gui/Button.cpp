#include "gui/Button.hpp"

Button::Button(Rectangle bounds, const std::string &text, Font *font)
    : bounds(bounds), text(text), font(font) {}

void Button::SetTextures(Texture2D *normal, Texture2D *hover, Texture2D *pressed)
{
    texNormal = normal;
    texHover = hover;
    texPressed = pressed;
}

void Button::SetSound(Sound *clickSound)
{
    this->clickSound = clickSound;
}

void Button::SetCallbacks(std::function<void()> onClick, std::function<void()> onHover, std::function<void()> onRelease)
{
    onClickCallback = onClick;
    onHoverCallback = onHover;
    onReleaseCallback = onRelease;
}

void Button::Update()
{
    Vector2 mouse = GetMousePosition();
    bool hovering = CheckCollisionPointRec(mouse, bounds);
    bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    if (hovering)
    {
        if (mousePressed)
        {
            state = PRESSED;
            wasPressed = true;
            if (clickSound)
                PlaySound(*clickSound);
            if (onClickCallback)
                onClickCallback();
        }
        else if (mouseDown)
        {
            state = PRESSED;
        }
        else
        {
            state = HOVERED;
            if (onHoverCallback)
                onHoverCallback();
            if (wasPressed && mouseReleased)
            {
                if (onReleaseCallback)
                    onReleaseCallback();
                wasPressed = false;
            }
        }
    }
    else
    {
        state = NORMAL;
        wasPressed = false;
    }
}

void Button::Draw()
{
    Texture2D *currentTexture = nullptr;
    switch (state)
    {
    case NORMAL:
        currentTexture = texNormal;
        break;
    case HOVERED:
        currentTexture = texHover;
        break;
    case PRESSED:
        currentTexture = texPressed;
        break;
    }

    if (currentTexture)
    {
        DrawTexturePro(*currentTexture, {0, 0, (float)currentTexture->width, (float)currentTexture->height},
                       bounds, {0, 0}, 0.0f, WHITE);
    }
    else
    {
        Color color;
        switch (state)
        {
        case NORMAL:
            color = GRAY;
            break;
        case HOVERED:
            color = DARKGRAY;
            break;
        case PRESSED:
            color = LIGHTGRAY;
            break;
        }
        DrawRectangleRec(bounds, color);
    }

    if (!text.empty())
    {
        int fontSize = 20;
        Vector2 textSize = font ? MeasureTextEx(*font, text.c_str(), (float)fontSize, 1.0f)
                                : Vector2{(float)MeasureText(text.c_str(), fontSize), (float)fontSize};
        Vector2 pos = {
            bounds.x + (bounds.width - textSize.x) / 2,
            bounds.y + (bounds.height - textSize.y) / 2};
        if (font)
            DrawTextEx(*font, text.c_str(), pos, (float)fontSize, 1.0f, BLACK);
        else
            DrawText(text.c_str(), (int)pos.x, (int)pos.y, fontSize, BLACK);
    }
}
