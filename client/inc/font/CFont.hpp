#pragma once

#include <memory>

#include "raylib-cpp.hpp"

class CFont : public raylib::Font
{
public:
    CFont();
    CFont(std::string path);
    ~CFont() = default;

    void setFontSpacing(const float spacing);
    void setFontSize(const float size);
    const float getFontSpacing();
    const float getFontSize();

private:
    std::string m_path;
    std::shared_ptr<raylib::Font> m_font;

    float m_fontSpacing;
    float m_fontSize;
};