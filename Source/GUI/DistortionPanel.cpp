#include "DistortionPanel.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

static juce::StringArray getDistortionNames()
{
    return {
        "Soft Clip", "Hard Clip", "Tube Sat", "Tape Sat", "Transistor", "Asymmetric",
        "Sine Fold", "Tri Fold", "Chebyshev", "Parabolic", "Exp Fold", "Multi Fold",
        "Noise Gate", "Noise Blend", "Ring Mod", "Noise AM", "Granular", "Static",
        "4-Bit", "8-Bit", "12-Bit", "Var Bit", "Stepped", "Quantize",
        "Decimate 2x", "Decimate 4x", "Decimate 8x", "Var Rate", "Jitter", "Drift"
    };
}

DistortionPanel::DistortionPanel(juce::AudioProcessorValueTreeState& apvts)
    : driveKnob(apvts, "distDrive", "DRIVE"),
      blendKnob(apvts, "distBlend", "BLEND")
{
    addAndMakeVisible(driveKnob);
    addAndMakeVisible(blendKnob);

    auto names = getDistortionNames();
    algoPosBox.addItemList(names, 1);
    algoNegBox.addItemList(names, 1);
    blendModeBox.addItemList({"Smooth", "Hard"}, 1);
    stereoModeBox.addItemList({"Stereo", "Mono", "Mid/Side"}, 1);

    addAndMakeVisible(algoPosBox);
    addAndMakeVisible(algoNegBox);
    addAndMakeVisible(blendModeBox);
    addAndMakeVisible(stereoModeBox);

    stagesSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    stagesSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 30, 20);
    stagesSlider.setColour(juce::Slider::textBoxTextColourId, Colors::textPrimary);
    stagesSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(stagesSlider);

    sectionLabel.setText("DISTORTION", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    sectionLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    sectionLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(sectionLabel);

    algoPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "distAlgoPos", algoPosBox);
    algoNegAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "distAlgoNeg", algoNegBox);
    blendModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "distBlendMode", blendModeBox);
    stereoModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "distStereoMode", stereoModeBox);
    stagesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "distStages", stagesSlider);
}

void DistortionPanel::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat());
}

void DistortionPanel::resized()
{
    auto bounds = getLocalBounds().reduced(12);
    sectionLabel.setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(8);

    auto topRow = bounds.removeFromTop(80);
    driveKnob.setBounds(topRow.removeFromLeft(topRow.getWidth() / 2));
    blendKnob.setBounds(topRow);

    bounds.removeFromTop(8);
    algoPosBox.setBounds(bounds.removeFromTop(28));
    bounds.removeFromTop(6);
    algoNegBox.setBounds(bounds.removeFromTop(28));
    bounds.removeFromTop(6);

    auto modeRow = bounds.removeFromTop(28);
    blendModeBox.setBounds(modeRow.removeFromLeft(modeRow.getWidth() / 2).reduced(0, 0));
    stereoModeBox.setBounds(modeRow);

    bounds.removeFromTop(6);
    stagesSlider.setBounds(bounds.removeFromTop(28));
}

}
