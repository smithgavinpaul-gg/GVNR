#include "MultibandPanel.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

MultibandPanel::MultibandPanel(juce::AudioProcessorValueTreeState& apvts)
    : lowFreqKnob(apvts, "mbLowFreq", "LOW FREQ"),
      highFreqKnob(apvts, "mbHighFreq", "HIGH FREQ")
{
    addAndMakeVisible(lowFreqKnob);
    addAndMakeVisible(highFreqKnob);

    enableButton.setButtonText("Multiband");
    addAndMakeVisible(enableButton);

    lowDist.setButtonText("Lo Dist");   midDist.setButtonText("Mid Dist");   highDist.setButtonText("Hi Dist");
    lowFilter.setButtonText("Lo Flt");  midFilter.setButtonText("Mid Flt");  highFilter.setButtonText("Hi Flt");
    lowFb.setButtonText("Lo FB");       midFb.setButtonText("Mid FB");       highFb.setButtonText("Hi FB");

    for (auto* b : {&lowDist, &midDist, &highDist, &lowFilter, &midFilter, &highFilter, &lowFb, &midFb, &highFb})
        addAndMakeVisible(b);

    sectionLabel.setText("MULTIBAND", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    sectionLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    sectionLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(sectionLabel);

    enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbEnabled", enableButton);
    lowDistAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbLowDist", lowDist);
    midDistAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbMidDist", midDist);
    highDistAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbHighDist", highDist);
    lowFilterAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbLowFilter", lowFilter);
    midFilterAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbMidFilter", midFilter);
    highFilterAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbHighFilter", highFilter);
    lowFbAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbLowFb", lowFb);
    midFbAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbMidFb", midFb);
    highFbAtt = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "mbHighFb", highFb);
}

void MultibandPanel::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat());
}

void MultibandPanel::resized()
{
    auto bounds = getLocalBounds().reduced(12);

    auto headerRow = bounds.removeFromTop(24);
    sectionLabel.setBounds(headerRow.removeFromLeft(headerRow.getWidth() / 2));
    enableButton.setBounds(headerRow);

    bounds.removeFromTop(8);
    auto knobRow = bounds.removeFromTop(80);
    lowFreqKnob.setBounds(knobRow.removeFromLeft(knobRow.getWidth() / 2));
    highFreqKnob.setBounds(knobRow);

    bounds.removeFromTop(8);
    int toggleH = 24;
    int colW = bounds.getWidth() / 3;

    auto row1 = bounds.removeFromTop(toggleH);
    lowDist.setBounds(row1.removeFromLeft(colW));
    midDist.setBounds(row1.removeFromLeft(colW));
    highDist.setBounds(row1);

    bounds.removeFromTop(4);
    auto row2 = bounds.removeFromTop(toggleH);
    lowFilter.setBounds(row2.removeFromLeft(colW));
    midFilter.setBounds(row2.removeFromLeft(colW));
    highFilter.setBounds(row2);

    bounds.removeFromTop(4);
    auto row3 = bounds.removeFromTop(toggleH);
    lowFb.setBounds(row3.removeFromLeft(colW));
    midFb.setBounds(row3.removeFromLeft(colW));
    highFb.setBounds(row3);
}

}
