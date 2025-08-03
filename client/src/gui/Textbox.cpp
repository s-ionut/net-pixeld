#include "gui/Textbox.hpp"

TextBox::TextBox(std::shared_ptr<raylib::Texture> texture,
                 std::shared_ptr<CFont> font,
                 std::shared_ptr<raylib::Sound> fx) : m_timer()
{
    m_texture = texture ? texture : std::make_shared<raylib::Texture>(CreateFallbackTexture());

    m_font = font ? font : std::make_shared<CFont>();
    if (!font)
    {
        m_font->setFontSize(kDefaultFontSize);
        m_font->setFontSpacing(kDefaultFontSpacing);
    }

    m_fx = std::move(fx);

    m_inputText = "";
    m_outputText = "-";
    m_frameHeight = static_cast<float>(m_texture->height) / m_numFrames;

    m_sourceRec = raylib::Rectangle(0, 0,
                                    static_cast<float>(m_texture->width),
                                    m_frameHeight);

    m_baseSize = raylib::Vector2(static_cast<float>(m_texture->width), m_frameHeight);
    m_position = raylib::Vector2(static_cast<float>(GetScreenWidth()) / 2,
                                 static_cast<float>(GetScreenHeight()) / 2);
}

void TextBox::SetCenter(const raylib::Vector2 &center)
{
    m_position = center;
}

void TextBox::SetDimensions(const raylib::Vector2 &dimensions)
{
    m_baseSize = dimensions;
}

void TextBox::SetFont(std::shared_ptr<CFont> font)
{
    m_font = std::move(font);
}

void TextBox::SetFocusAnimation(bool enabled)
{
    m_animateFocus = enabled;
}

void TextBox::Update()
{
    UpdateState();
    if (m_state == TextBoxState::ACTIVE)
        HandleInput();
    UpdateOutputText();
    Animate();
}

void TextBox::Draw()
{
    m_sourceRec.y = static_cast<float>(static_cast<int>(m_state)) * m_frameHeight;

    raylib::Rectangle bounds = GetTransformedBounds();
    DrawTexturePro(*m_texture, m_sourceRec, bounds, {0, 0}, 0.0f, WHITE);

    raylib::Vector2 textSize = m_font->MeasureText(
        m_outputText, m_font->getFontSize(), m_font->getFontSpacing());

    raylib::Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2};

    m_font->DrawText(m_outputText, textPos,
                     m_font->getFontSize(), m_font->getFontSpacing(), RED);
}

void TextBox::HandleInput()
{
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            m_inputText += static_cast<char>(key);
            if (m_fx)
                m_fx->Play();
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !m_inputText.empty())
    {
        m_inputText.pop_back();
    }
}

void TextBox::UpdateState()
{
    raylib::Rectangle bounds = GetTransformedBounds();
    raylib::Vector2 mousePos = raylib::Mouse::GetPosition();
    bool hovered = bounds.CheckCollision(mousePos);

    if (hovered)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            m_state = TextBoxState::ACTIVE;
        else if (m_state != TextBoxState::ACTIVE)
            m_state = TextBoxState::HOVERED;
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        m_state = TextBoxState::NOT_ACTIVE;
    }
    else if (m_state == TextBoxState::HOVERED)
    {
        m_state = TextBoxState::NOT_ACTIVE;
    }
}

void TextBox::Animate()
{
    if (!m_animateFocus)
        return;

    float targetScale = (m_state == TextBoxState::HOVERED || m_state == TextBoxState::ACTIVE) ? kScaleHover : 1.0f;
    m_scale += (targetScale - m_scale) * kScaleSpeed;
}

void TextBox::UpdateOutputText()
{
    raylib::Rectangle bounds = GetTransformedBounds();
    float maxWidth = bounds.width - 10.0f;

    m_font->setFontSize(kDefaultFontSize); // start large
    m_font->setFontSpacing(kDefaultFontSpacing);

    std::string displayText = m_inputText;

    while (m_font->MeasureText(displayText,
                               m_font->getFontSize(),
                               m_font->getFontSpacing())
                   .x > maxWidth &&
           m_font->getFontSize() > 8)
    {
        m_font->setFontSize(m_font->getFontSize() - 1);
    }

    m_outputText = displayText;

    if (m_showingCursor)
        m_outputText += "|";

    if (m_timer.hasTimePassedMS(kBlinkInterval))
    {
        m_showingCursor = !m_showingCursor;
        m_timer.refresh();
    }

    if (m_outputText.empty())
        m_outputText = "-";
}

raylib::Rectangle TextBox::GetTransformedBounds() const
{
    raylib::Vector2 size = m_baseSize * m_scale;
    return raylib::Rectangle(m_position.x - size.x / 2, m_position.y - size.y / 2,
                             size.x, size.y);
}

std::string TextBox::GetText() const
{
    return m_inputText;
}

raylib::Texture TextBox::CreateFallbackTexture()
{
    const int barWidth = kDefaultWidth;
    const int barHeight = kDefaultHeight;
    std::vector<Color> colors = {DARKGRAY, GRAY, LIGHTGRAY};

    Image image = GenImageColor(barWidth, barHeight * colors.size(), BLACK);

    for (size_t i = 0; i < colors.size(); ++i)
    {
        Rectangle rec = {0, static_cast<float>(i * barHeight),
                         static_cast<float>(barWidth), static_cast<float>(barHeight)};
        ImageDrawRectangleRec(&image, rec, colors[i]);
    }

    Texture2D tex = LoadTextureFromImage(image);
    UnloadImage(image);
    return tex;
}