#include "FeedbackPanel.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

FeedbackPanel::FeedbackPanel(juce::AudioProcessorValueTreeState& apvts)
    : timeKnob(apvts, "fbTime", "TIME"),
      feedbackKnob(apvts, "fbFeedback", "FEEDBACK")
{
    addAndMakeVisible(timeKnob);
    addAndMakeVisible(feedbackKnob);

    enableButton.setButtonText("Enable");
    pingPongButton.setButtonText("Ping Pong");
    syncButton.setButtonText("Sync");
    pitchSnapButton.setButtonText("Pitch Snap");
    addAndMakeVisible(enableButton);
    addAndMakeVisible(pingPongButton);
    addAndMakeVisible(syncButton);
    addAndMakeVisible(pitchSnapButton);

    syncRateBox.addItemList({"1/16", "1/8", "1/4", "1/2", "1/1", "1/16T", "1/8T", "1/4T", "1/16D", "1/8D", "1/4D"}, 1);
    scaleBox.addItemList({"Chromatic", "Major", "Minor", "Pent Maj", "Pent Min", "Blues", "Dorian", "Mixolydian", "Harm Min", "Whole Tone"}, 1);
    keyBox.addItemList({"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}, 1);
    addAndMakeVisible(syncRateBox);
    addAndMakeVisible(scaleBox);
    addAndMakeVisible(keyBox);

    sectionLabel.setText("FEEDBACK / DELAY", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    sectionLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    sectionLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(sectionLabel);

    enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "fbEnabled", enableButton);
    pingPongAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "fbPingPong", pingPongButton);
    syncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "fbSync", syncButton);
    pitchSnapAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "fbPitchSnap", pitchSnapButton);
    syncRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "fbSyncRate", syncRateBox);
    scaleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "fbScale", scaleBox);
    keyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "fbKey", keyBox);
}

void FeedbackPanel::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat());
}

void FeedbackPanel::resized()
{
    auto bounds = getLocalBounds().reduced(12);

    auto headerRow = bounds.removeFromTop(24);
    sectionLabel.setBounds(headerRow.removeFromLeft(headerRow.getWidth() * 2 / 3));
    enableButton.setBounds(headerRow);

    bounds.removeFromTop(6);
    auto knobRow = bounds.removeFromTop(80);
    timeKnob.setBounds(knobRow.removeFromLeft(knobRow.getWidth() / 2));
    feedbackKnob.setBounds(knobRow);

    bounds.removeFromTop(6);
    auto toggleRow1 = bounds.removeFromTop(24);
    pingPongButton.setBounds(toggleRow1.removeFromLeft(toggleRow1.getWidth() / 2));
    syncButton.setBounds(toggleRow1);

    bounds.removeFromTop(6);
    syncRateBox.setBounds(bounds.removeFromTop(28));

    bounds.removeFromTop(6);
    pitchSnapButton.setBounds(bounds.removeFromTop(24));

    bounds.removeFromTop(6);
    auto pitchRow = bounds.removeFromTop(28);
    keyBox.setBounds(pitchRow.removeFromLeft(pitchRow.getWidth() / 2 - 3));
    pitchRow.removeFromLeft(6);
    scaleBox.setBounds(pitchRow);
}

}
