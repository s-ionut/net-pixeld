#pragma once

#include "Logger.hpp"

#include "font/CFont.hpp"
#include "app/Timer.hpp"

#include "raylib-cpp.hpp"

constexpr float kScaleHover = 1.05f;
constexpr float kScaleSpeed = 0.15f;
constexpr float kBlinkInterval = 1000.0f;
constexpr float kDefaultFontSize = 20.0f;
constexpr float kDefaultFontSpacing = 2.0f;
constexpr int kDefaultWidth = 180;
constexpr int kDefaultHeight = 36;

enum class TextBoxState
{
    NOT_ACTIVE = 0,
    HOVERED,
    ACTIVE,
    COUNT
};

class TextBox
{
public:
    TextBox(std::shared_ptr<raylib::Texture> texture = nullptr,
            std::shared_ptr<CFont> font = nullptr,
            std::shared_ptr<raylib::Sound> fx = nullptr);

    void Update();
    void Draw();

    void SetCenter(const raylib::Vector2 &center);
    void SetDimensions(const raylib::Vector2 &dimensions);
    void SetFont(std::shared_ptr<CFont> font);
    void SetFocusAnimation(bool enabled);

    std::string GetText() const;

private:
    void HandleInput();
    void UpdateState();
    void Animate();
    void UpdateOutputText();
    raylib::Texture CreateFallbackTexture();
    raylib::Rectangle GetTransformedBounds() const;

private:
    std::shared_ptr<raylib::Texture> m_texture;
    std::shared_ptr<CFont> m_font;
    std::shared_ptr<raylib::Sound> m_fx;

    std::string m_inputText;
    std::string m_outputText;

    raylib::Vector2 m_position; // Center position
    raylib::Vector2 m_baseSize; // Original size

    raylib::Rectangle m_sourceRec;

    TextBoxState m_state = TextBoxState::NOT_ACTIVE;
    int m_numFrames = static_cast<int>(TextBoxState::COUNT);
    float m_frameHeight = 0.0f;
    float m_scale = 1.0f;
    bool m_animateFocus = true;
    bool m_showingCursor = true;

    Timer m_timer;
};