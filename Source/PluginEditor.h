#pragma once

#include "PluginProcessor.h"
#include "GUI/GVNRLookAndFeel.h"
#include "GUI/MainPanel.h"

class TheGVNREditor : public juce::AudioProcessorEditor
{
public:
    explicit TheGVNREditor(TheGVNRProcessor&);
    ~TheGVNREditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    TheGVNRProcessor& processorRef;
    gvnr::GVNRLookAndFeel lookAndFeel;
    gvnr::MainPanel mainPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TheGVNREditor)
};
