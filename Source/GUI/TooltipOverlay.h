#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace gvnr {

class TooltipOverlay : public juce::Component, public juce::Timer {
public:
    TooltipOverlay();
    ~TooltipOverlay() override;

    void showTooltip(const juce::String& text, juce::Point<int> position);
    void hideTooltip();

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

private:
    juce::String tooltipText;
    juce::Point<int> tooltipPosition;
    bool visible = false;
    float alpha = 0.0f;
};

}
