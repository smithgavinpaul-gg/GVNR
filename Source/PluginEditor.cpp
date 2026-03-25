#include "PluginEditor.h"

TheGVNREditor::TheGVNREditor(TheGVNRProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      mainPanel(p, lookAndFeel)
{
    setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(mainPanel);
    setSize(1000, 700);
    setResizable(true, true);
    setResizeLimits(800, 560, 1600, 1120);
    getConstrainer()->setFixedAspectRatio(10.0 / 7.0);
}

TheGVNREditor::~TheGVNREditor()
{
    setLookAndFeel(nullptr);
}

void TheGVNREditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient bgGrad(juce::Colour(0xff080810), bounds.getCentreX(), bounds.getY(),
                                 juce::Colour(0xff0a0a14), bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill(bgGrad);
    g.fillRect(bounds);
}

void TheGVNREditor::resized()
{
    mainPanel.setBounds(getLocalBounds());
}
