#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace gvnr {

struct ThemeColors {
    juce::Colour background;
    juce::Colour surface;
    juce::Colour surfaceElevated;
    juce::Colour accent;
    juce::Colour accentDim;
    juce::Colour highlight;
    juce::Colour textPrimary;
    juce::Colour textSecondary;
    juce::Colour divider;
    juce::Colour knobTrack;
};

inline const ThemeColors darkTheme {
    juce::Colour(0xff080810), juce::Colour(0xff1a1a2e), juce::Colour(0xff252540),
    juce::Colour(0xffff66c4), juce::Colour(0xffd94fa3), juce::Colour(0xff9b5cf6),
    juce::Colour(0xfff5f5f7), juce::Colour(0xff86868b),
    juce::Colour(0xff2d2d3a), juce::Colour(0xff3a3a4a)
};

inline const ThemeColors lightTheme {
    juce::Colour(0xfff5f5f7), juce::Colour(0xffffffff), juce::Colour(0xffe8e8ed),
    juce::Colour(0xffff66c4), juce::Colour(0xffd94fa3), juce::Colour(0xff7c3aed),
    juce::Colour(0xff1a1a1a), juce::Colour(0xff6b6b70),
    juce::Colour(0xffd4d4d8), juce::Colour(0xffc0c0c5)
};

struct Colors {
    static inline juce::Colour background      = juce::Colour(0xff080810);
    static inline juce::Colour surface          = juce::Colour(0xff1a1a2e);
    static inline juce::Colour surfaceElevated  = juce::Colour(0xff252540);
    static inline juce::Colour accent           = juce::Colour(0xffff66c4);
    static inline juce::Colour accentDim        = juce::Colour(0xffd94fa3);
    static inline juce::Colour highlight        = juce::Colour(0xff9b5cf6);
    static inline juce::Colour textPrimary      = juce::Colour(0xfff5f5f7);
    static inline juce::Colour textSecondary    = juce::Colour(0xff86868b);
    static inline juce::Colour divider          = juce::Colour(0xff2d2d3a);
    static inline juce::Colour knobTrack        = juce::Colour(0xff3a3a4a);

    static inline juce::Colour glassBorder      = juce::Colour(0x20ffffff);
    static inline juce::Colour glassHighlight   = juce::Colour(0x18ffffff);
    static inline juce::Colour accentGlow       = juce::Colour(0x30ff66c4);

    static void applyTheme(const ThemeColors& theme) {
        background     = theme.background;
        surface        = theme.surface;
        surfaceElevated = theme.surfaceElevated;
        accent         = theme.accent;
        accentDim      = theme.accentDim;
        highlight      = theme.highlight;
        textPrimary    = theme.textPrimary;
        textSecondary  = theme.textSecondary;
        divider        = theme.divider;
        knobTrack      = theme.knobTrack;
    }
};

struct GlassPanel {
    static void paint(juce::Graphics& g, juce::Rectangle<float> bounds, float cornerRadius = 12.0f)
    {
        g.setColour(Colors::surface.withAlpha(0.6f));
        g.fillRoundedRectangle(bounds, cornerRadius);

        juce::ColourGradient topGlow(Colors::glassHighlight, bounds.getX(), bounds.getY(),
                                      juce::Colour(0x00ffffff), bounds.getX(), bounds.getY() + 40.0f, false);
        g.setGradientFill(topGlow);
        g.fillRoundedRectangle(bounds, cornerRadius);

        g.setColour(Colors::glassBorder);
        g.drawRoundedRectangle(bounds.reduced(0.5f), cornerRadius, 1.0f);
    }
};

class GVNRLookAndFeel : public juce::LookAndFeel_V4 {
public:
    GVNRLookAndFeel();

    void setDarkMode(bool dark);
    bool isDarkMode() const { return darkMode; }

    void applyThemeColours();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    juce::Font getComboBoxFont(juce::ComboBox& box) override;
    juce::Font getPopupMenuFont() override;
    juce::Font getLabelFont(juce::Label& label) override;

private:
    bool darkMode = true;
};

}
