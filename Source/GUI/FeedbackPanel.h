#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"

namespace gvnr {

class ModulationMatrix;

class FeedbackPanel : public juce::Component {
public:
    FeedbackPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix) {
        timeKnob.setModulationMatrix(matrix);
        feedbackKnob.setModulationMatrix(matrix);
    }

private:
    KnobComponent timeKnob;
    KnobComponent feedbackKnob;

    juce::ToggleButton enableButton;
    juce::ToggleButton pingPongButton;
    juce::ToggleButton syncButton;
    juce::ToggleButton pitchSnapButton;

    juce::ComboBox syncRateBox;
    juce::ComboBox scaleBox;
    juce::ComboBox keyBox;

    juce::Label sectionLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pingPongAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pitchSnapAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syncRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> scaleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> keyAttachment;
};

}
