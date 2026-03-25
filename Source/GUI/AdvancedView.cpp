#include "AdvancedView.h"
#include "GVNRLookAndFeel.h"
#include "../PluginProcessor.h"

namespace gvnr {

AdvancedView::AdvancedView(TheGVNRProcessor& processor)
    : distortionPanel(processor.getAPVTS()),
      preFilterPanel(processor.getAPVTS(), true),
      postFilterPanel(processor.getAPVTS(), false),
      feedbackPanel(processor.getAPVTS()),
      multibandPanel(processor.getAPVTS()),
      modulationPanel(processor.getAPVTS())
{
    addAndMakeVisible(distortionPanel);
    addAndMakeVisible(preFilterPanel);
    addAndMakeVisible(postFilterPanel);
    addAndMakeVisible(feedbackPanel);
    addAndMakeVisible(multibandPanel);
    addAndMakeVisible(modulationPanel);
    addAndMakeVisible(waveformDisplay);

    titleLabel.setText("GVNRs Grunger", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(20.0f).withStyle("Bold")));
    titleLabel.setColour(juce::Label::textColourId, Colors::accent);
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    waveformDisplay.setSource(processor.getWaveformData(),
                               &processor.getWaveformWritePosAtomic(),
                               TheGVNRProcessor::kWaveformBufferSize);
}

void AdvancedView::setModulationMatrix(ModulationMatrix* matrix)
{
    distortionPanel.setModulationMatrix(matrix);
    preFilterPanel.setModulationMatrix(matrix);
    postFilterPanel.setModulationMatrix(matrix);
    feedbackPanel.setModulationMatrix(matrix);
    multibandPanel.setModulationMatrix(matrix);
    modulationPanel.setModulationMatrix(matrix);
}

void AdvancedView::paint(juce::Graphics& g)
{
    g.fillAll(Colors::background);
}

void AdvancedView::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    auto topBar = bounds.removeFromTop(32);
    titleLabel.setBounds(topBar);

    bounds.removeFromTop(8);
    auto waveArea = bounds.removeFromTop(100);
    waveformDisplay.setBounds(waveArea);

    bounds.removeFromTop(10);

    int gap = 10;

    // Main content: 3 columns with uniform gaps
    int colWidth = (bounds.getWidth() - gap * 2) / 3;
    auto leftCol = bounds.removeFromLeft(colWidth);
    bounds.removeFromLeft(gap);
    auto midCol = bounds.removeFromLeft(colWidth);
    bounds.removeFromLeft(gap);
    auto rightCol = bounds;

    // Left column: distortion + pre-filter
    distortionPanel.setBounds(leftCol.removeFromTop(leftCol.getHeight() / 2 - gap / 2));
    leftCol.removeFromTop(gap);
    preFilterPanel.setBounds(leftCol);

    // Middle column: feedback + multiband
    feedbackPanel.setBounds(midCol.removeFromTop(midCol.getHeight() / 2 - gap / 2));
    midCol.removeFromTop(gap);
    multibandPanel.setBounds(midCol);

    // Right column: post-filter + modulation
    postFilterPanel.setBounds(rightCol.removeFromTop(rightCol.getHeight() / 2 - gap / 2));
    rightCol.removeFromTop(gap);
    modulationPanel.setBounds(rightCol);
}

}
