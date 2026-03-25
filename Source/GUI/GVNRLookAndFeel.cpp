#include "GVNRLookAndFeel.h"

#include <algorithm>
#include <cmath>

namespace gvnr {

GVNRLookAndFeel::GVNRLookAndFeel()
{
    applyThemeColours();
}

void GVNRLookAndFeel::setDarkMode(bool dark)
{
    darkMode = dark;
    Colors::applyTheme(dark ? darkTheme : lightTheme);
    applyThemeColours();
}

void GVNRLookAndFeel::applyThemeColours()
{
    setColour(juce::ResizableWindow::backgroundColourId, Colors::background);
    setColour(juce::Slider::thumbColourId, Colors::accent);
    setColour(juce::Slider::rotarySliderFillColourId, Colors::accent);
    setColour(juce::Slider::rotarySliderOutlineColourId, Colors::knobTrack);
    setColour(juce::Slider::trackColourId, Colors::knobTrack);
    setColour(juce::Label::textColourId, Colors::textPrimary);
    setColour(juce::ComboBox::backgroundColourId, Colors::surface);
    setColour(juce::ComboBox::textColourId, Colors::textPrimary);
    setColour(juce::ComboBox::outlineColourId, Colors::divider);
    setColour(juce::ComboBox::arrowColourId, Colors::textSecondary);
    setColour(juce::PopupMenu::backgroundColourId, Colors::surfaceElevated);
    setColour(juce::PopupMenu::textColourId, Colors::textPrimary);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, Colors::accent);
    setColour(juce::PopupMenu::highlightedTextColourId, Colors::background);
    setColour(juce::TextButton::buttonColourId, Colors::surface);
    setColour(juce::TextButton::textColourOffId, Colors::textPrimary);
    setColour(juce::TextButton::buttonOnColourId, Colors::accent);
    setColour(juce::TextButton::textColourOnId, Colors::background);
    setColour(juce::ToggleButton::textColourId, Colors::textPrimary);
    setColour(juce::ToggleButton::tickColourId, Colors::accent);
}

void GVNRLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPos, float rotaryStartAngle,
                                        float rotaryEndAngle, juce::Slider&)
{
    float diameter = static_cast<float>(std::min(width, height)) * 0.8f;
    float radius = diameter * 0.5f;
    float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
    float rx = centreX - radius;
    float ry = centreY - radius;

    float trackWidth = 4.0f;
    float arcRadius = radius - trackWidth * 0.5f;

    // Drop shadow
    g.setColour(juce::Colour(0x30000000));
    g.fillEllipse(rx + 1.0f, ry + 2.0f, diameter, diameter);

    // Knob cap with radial gradient for 3D depth
    juce::ColourGradient knobGrad(Colors::surfaceElevated.brighter(0.15f), centreX, centreY - radius * 0.3f,
                                   Colors::surface.darker(0.2f), centreX, centreY + radius, true);
    g.setGradientFill(knobGrad);
    g.fillEllipse(rx, ry, diameter, diameter);

    // Chrome bezel ring
    g.setColour(juce::Colour(0x15ffffff));
    g.drawEllipse(rx, ry, diameter, diameter, 1.5f);

    // Track arc (background)
    juce::Path trackArc;
    trackArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f,
                           rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(Colors::knobTrack);
    g.strokePath(trackArc, juce::PathStrokeType(trackWidth, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));

    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Neon glow behind value arc
    juce::Path valueArc;
    valueArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f,
                           rotaryStartAngle, angle, true);
    g.setColour(Colors::accent.withAlpha(0.15f));
    g.strokePath(valueArc, juce::PathStrokeType(trackWidth + 4.0f, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));

    // Value arc
    g.setColour(Colors::accent);
    g.strokePath(valueArc, juce::PathStrokeType(trackWidth, juce::PathStrokeType::curved,
                                                 juce::PathStrokeType::rounded));

    // Dot indicator
    float dotRadius = 3.0f;
    float dotDistance = arcRadius * 0.7f;
    float dotX = centreX + dotDistance * std::cos(angle - juce::MathConstants<float>::halfPi);
    float dotY = centreY + dotDistance * std::sin(angle - juce::MathConstants<float>::halfPi);
    g.setColour(Colors::accent);
    g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);
}

void GVNRLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPos, float, float,
                                        juce::Slider::SliderStyle style, juce::Slider&)
{
    float trackHeight = 4.0f;

    if (style == juce::Slider::LinearHorizontal) {
        float trackY = static_cast<float>(y) + static_cast<float>(height) * 0.5f - trackHeight * 0.5f;

        g.setColour(Colors::knobTrack);
        g.fillRoundedRectangle(static_cast<float>(x), trackY, static_cast<float>(width), trackHeight, 2.0f);

        g.setColour(Colors::accent);
        g.fillRoundedRectangle(static_cast<float>(x), trackY, sliderPos - static_cast<float>(x), trackHeight, 2.0f);

        g.setColour(Colors::accent);
        g.fillEllipse(sliderPos - 6.0f, trackY - 4.0f, 12.0f, 12.0f);
    }
}

void GVNRLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour&,
                                             bool isHighlighted, bool isDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);

    if (button.getToggleState()) {
        juce::ColourGradient btnGrad(isDown ? Colors::accentDim : Colors::accent.brighter(0.1f),
                                      bounds.getX(), bounds.getY(),
                                      isDown ? Colors::accentDim.darker(0.1f) : Colors::accent.darker(0.1f),
                                      bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill(btnGrad);
    } else {
        juce::ColourGradient btnGrad(Colors::surfaceElevated.brighter(isHighlighted ? 0.1f : 0.05f),
                                      bounds.getX(), bounds.getY(),
                                      Colors::surface.darker(isDown ? 0.1f : 0.0f),
                                      bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill(btnGrad);
    }

    g.fillRoundedRectangle(bounds, 8.0f);
    g.setColour(Colors::glassBorder);
    g.drawRoundedRectangle(bounds, 8.0f, 1.0f);
}

void GVNRLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                        bool isHighlighted, bool)
{
    auto bounds = button.getLocalBounds().toFloat();
    float toggleWidth = 40.0f;
    float toggleHeight = 22.0f;
    float toggleX = bounds.getX() + 4.0f;
    float toggleY = bounds.getCentreY() - toggleHeight * 0.5f;

    juce::Colour trackColour = button.getToggleState() ? Colors::accent : Colors::knobTrack;
    if (isHighlighted) trackColour = trackColour.brighter(0.1f);

    if (button.getToggleState()) {
        g.setColour(Colors::accent.withAlpha(0.2f));
        g.fillRoundedRectangle(toggleX - 3.0f, toggleY - 3.0f,
                                toggleWidth + 6.0f, toggleHeight + 6.0f,
                                (toggleHeight + 6.0f) * 0.5f);
    }

    g.setColour(trackColour);
    g.fillRoundedRectangle(toggleX, toggleY, toggleWidth, toggleHeight, toggleHeight * 0.5f);

    float dotDiameter = toggleHeight - 4.0f;
    float dotX = button.getToggleState() ? (toggleX + toggleWidth - dotDiameter - 2.0f) : (toggleX + 2.0f);
    g.setColour(Colors::textPrimary);
    g.fillEllipse(dotX, toggleY + 2.0f, dotDiameter, dotDiameter);

    g.setColour(Colors::textPrimary);
    g.setFont(juce::Font(juce::FontOptions(14.0f)));
    g.drawText(button.getButtonText(),
               juce::Rectangle<float>(toggleX + toggleWidth + 8.0f, bounds.getY(),
                                       bounds.getWidth() - toggleWidth - 12.0f, bounds.getHeight()),
               juce::Justification::centredLeft);
}

void GVNRLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool,
                                    int, int, int, int, juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float>(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));

    g.setColour(Colors::surface.withAlpha(0.6f));
    g.fillRoundedRectangle(bounds, 8.0f);

    juce::ColourGradient topGlow(juce::Colour(0x10ffffff), bounds.getX(), bounds.getY(),
                                  juce::Colour(0x00ffffff), bounds.getX(), bounds.getY() + 20.0f, false);
    g.setGradientFill(topGlow);
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(box.hasKeyboardFocus(true) ? Colors::accentGlow : Colors::glassBorder);
    g.drawRoundedRectangle(bounds, 8.0f, 1.0f);

    float arrowX = static_cast<float>(width) - 20.0f;
    float arrowY = static_cast<float>(height) * 0.5f;
    juce::Path arrow;
    arrow.addTriangle(arrowX - 4.0f, arrowY - 2.0f, arrowX + 4.0f, arrowY - 2.0f, arrowX, arrowY + 3.0f);
    g.setColour(Colors::textSecondary);
    g.fillPath(arrow);
}

void GVNRLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
    auto bounds = juce::Rectangle<float>(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    GlassPanel::paint(g, bounds, 8.0f);
}

void GVNRLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(label.findColour(juce::Label::textColourId));
    g.setFont(getLabelFont(label));
    auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
    g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                     std::max(1, static_cast<int>(static_cast<float>(textArea.getHeight()) / 14.0f)),
                     label.getMinimumHorizontalScale());
}

juce::Font GVNRLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return juce::Font(juce::FontOptions(14.0f));
}

juce::Font GVNRLookAndFeel::getPopupMenuFont()
{
    return juce::Font(juce::FontOptions(14.0f));
}

juce::Font GVNRLookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font(juce::FontOptions(12.0f));
}

}
