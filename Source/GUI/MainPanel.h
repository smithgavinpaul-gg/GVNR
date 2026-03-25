#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PlayView.h"
#include "AdvancedView.h"
#include "PresetBrowser.h"
#include "TooltipOverlay.h"

class TheGVNRProcessor;

namespace gvnr {

class GVNRLookAndFeel;

class MainPanel : public juce::Component,
                  public juce::DragAndDropContainer {
public:
    MainPanel(TheGVNRProcessor& processor, GVNRLookAndFeel& lnf);
    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    TheGVNRProcessor& processor;
    GVNRLookAndFeel& lookAndFeel;

    juce::Image logo;
    juce::Image background;

    PlayView playView;
    AdvancedView advancedView;
    PresetBrowser presetBrowser;
    TooltipOverlay tooltip;

    juce::TextButton playViewBtn;
    juce::TextButton advancedViewBtn;
    juce::TextButton themeToggleBtn;

    bool showAdvanced = false;
};

}
