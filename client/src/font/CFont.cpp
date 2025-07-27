#include "font/CFont.hpp"

CFont::CFont() = default;

CFont::CFont(const std::string &path)
    : m_path(path), m_fontSpacing(1.0f), m_fontSize(20.0f)
{
    try
    {
        raylib::Font f = ::LoadFontEx(path.c_str(), static_cast<int>(m_fontSize), nullptr, 0);
        if (f.texture.id == 0)
        {
            throw raylib::RaylibException("Failed to load FontEx: " + m_path);
        }

        SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
        static_cast<raylib::Font &>(*this) = std::move(f);
    }
    catch (const raylib::RaylibException &error)
    {
        LOG_ERROR("Failed to load font: %s", error.what());
    }
}

void CFont::setFontSpacing(float spacing)
{
    m_fontSpacing = spacing;
}

void CFont::setFontSize(float size)
{
    m_fontSize = size;
}

float CFont::getFontSpacing() const
{
    return m_fontSpacing;
}

float CFont::getFontSize() const
{
    return m_fontSize;
}
