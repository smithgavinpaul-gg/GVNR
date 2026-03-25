#include "TooltipOverlay.h"
#include "GVNRLookAndFeel.h"
#include <algorithm>

namespace gvnr {

TooltipOverlay::TooltipOverlay()
{
    setInterceptsMouseClicks(false, false);
    setAlwaysOnTop(true);
}

TooltipOverlay::~TooltipOverlay()
{
    stopTimer();
}

void TooltipOverlay::showTooltip(const juce::String& text, juce::Point<int> position)
{
    tooltipText = text;
    tooltipPosition = position;
    visible = true;
    alpha = 0.0f;
    startTimerHz(60);
    repaint();
}

void TooltipOverlay::hideTooltip()
{
    visible = false;
    stopTimer();
    repaint();
}

void TooltipOverlay::timerCallback()
{
    if (visible && alpha < 1.0f) {
        alpha = std::min(alpha + 0.1f, 1.0f);
        repaint();
    }
}

void TooltipOverlay::paint(juce::Graphics& g)
{
    if (!visible || tooltipText.isEmpty()) return;

    g.setOpacity(alpha);

    juce::Font font(juce::FontOptions(12.0f));
    juce::GlyphArrangement glyphs;
    glyphs.addLineOfText(font, tooltipText, 0.0f, 0.0f);
    float textWidth = glyphs.getBoundingBox(0, glyphs.getNumGlyphs(), false).getWidth() + 16.0f;
    float textHeight = 28.0f;

    float x = static_cast<float>(tooltipPosition.x) - textWidth * 0.5f;
    float y = static_cast<float>(tooltipPosition.y) - textHeight - 8.0f;

    x = std::clamp(x, 4.0f, static_cast<float>(getWidth()) - textWidth - 4.0f);
    y = std::max(y, 4.0f);

    GlassPanel::paint(g, juce::Rectangle<float>(x, y, textWidth, textHeight), 6.0f);

    g.setColour(Colors::textPrimary);
    g.setFont(font);
    g.drawText(tooltipText, static_cast<int>(x), static_cast<int>(y),
               static_cast<int>(textWidth), static_cast<int>(textHeight),
               juce::Justification::centred);
}

}
