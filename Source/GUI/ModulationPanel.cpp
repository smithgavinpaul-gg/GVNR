#include "ModulationPanel.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

ModulationPanel::ModulationPanel(juce::AudioProcessorValueTreeState& apvts)
    : envAttackKnob(apvts, "modEnvAttack", "ATTACK"),
      envReleaseKnob(apvts, "modEnvRelease", "RELEASE"),
      lfoRateKnob(apvts, "modLfoRate", "RATE"),
      lfoMorphKnob(apvts, "modLfoMorph", "MORPH"),
      macro1Knob(apvts, "macro1", "MACRO 1"),
      macro2Knob(apvts, "macro2", "MACRO 2")
{
    addAndMakeVisible(envAttackKnob);
    addAndMakeVisible(envReleaseKnob);
    addAndMakeVisible(lfoRateKnob);
    addAndMakeVisible(lfoMorphKnob);
    addAndMakeVisible(macro1Knob);
    addAndMakeVisible(macro2Knob);

    lfoShapeBox.addItemList({"Sine", "Triangle", "Saw Up", "Saw Down", "Square", "Random S&H"}, 1);
    addAndMakeVisible(lfoShapeBox);

    lfoSyncButton.setButtonText("Sync");
    addAndMakeVisible(lfoSyncButton);

    auto setupLabel = [this](juce::Label& l, const juce::String& text) {
        l.setText(text, juce::dontSendNotification);
        l.setFont(juce::Font(juce::FontOptions(10.0f)));
        l.setColour(juce::Label::textColourId, Colors::textSecondary);
        l.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(l);
    };

    sectionLabel.setText("MODULATION", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    sectionLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    sectionLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(sectionLabel);

    setupLabel(envLabel, "ENVELOPE");
    setupLabel(lfoLabel, "LFO");
    setupLabel(macroLabel, "MACROS");

    addAndMakeVisible(envDragHandle);
    addAndMakeVisible(lfoDragHandle);
    addAndMakeVisible(macro1DragHandle);
    addAndMakeVisible(macro2DragHandle);

    lfoShapeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "modLfoShape", lfoShapeBox);
    lfoSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "modLfoSync", lfoSyncButton);
}

void ModulationPanel::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat());
}

void ModulationPanel::resized()
{
    auto bounds = getLocalBounds().reduced(12);
    sectionLabel.setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(4);

    auto envLabelRow = bounds.removeFromTop(16);
    envDragHandle.setBounds(envLabelRow.removeFromRight(28).reduced(0, 1));
    envLabel.setBounds(envLabelRow);
    auto envRow = bounds.removeFromTop(70);
    envAttackKnob.setBounds(envRow.removeFromLeft(envRow.getWidth() / 2));
    envReleaseKnob.setBounds(envRow);

    bounds.removeFromTop(4);
    auto lfoLabelRow = bounds.removeFromTop(16);
    lfoDragHandle.setBounds(lfoLabelRow.removeFromRight(28).reduced(0, 1));
    lfoLabel.setBounds(lfoLabelRow);

    auto lfoControlRow = bounds.removeFromTop(24);
    lfoShapeBox.setBounds(lfoControlRow.removeFromLeft(lfoControlRow.getWidth() * 2 / 3));
    lfoSyncButton.setBounds(lfoControlRow);

    bounds.removeFromTop(4);
    auto lfoKnobRow = bounds.removeFromTop(70);
    lfoRateKnob.setBounds(lfoKnobRow.removeFromLeft(lfoKnobRow.getWidth() / 2));
    lfoMorphKnob.setBounds(lfoKnobRow);

    bounds.removeFromTop(4);
    auto macroLabelRow = bounds.removeFromTop(16);
    macro2DragHandle.setBounds(macroLabelRow.removeFromRight(28).reduced(0, 1));
    macroLabelRow.removeFromRight(4);
    macro1DragHandle.setBounds(macroLabelRow.removeFromRight(28).reduced(0, 1));
    macroLabel.setBounds(macroLabelRow);
    auto macroRow = bounds.removeFromTop(70);
    macro1Knob.setBounds(macroRow.removeFromLeft(macroRow.getWidth() / 2));
    macro2Knob.setBounds(macroRow);
}

}
