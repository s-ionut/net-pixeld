#pragma once

#include "Logger.hpp"

#include "raylib-cpp.hpp"

#include <string>
#include <memory>

class CFont : public raylib::Font
{
public:
    CFont();
    explicit CFont(const std::string &path);
    ~CFont() = default;

    void setFontSpacing(float spacing);
    void setFontSize(float size);

    float getFontSpacing() const;
    float getFontSize() const;

private:
    std::string m_path;

    float m_fontSpacing;
    float m_fontSize;
};
