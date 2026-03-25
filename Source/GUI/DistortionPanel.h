#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"

namespace gvnr {

class ModulationMatrix;

class DistortionPanel : public juce::Component {
public:
    DistortionPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix) {
        driveKnob.setModulationMatrix(matrix);
        blendKnob.setModulationMatrix(matrix);
    }

private:
    KnobComponent driveKnob;
    KnobComponent blendKnob;

    juce::ComboBox algoPosBox;
    juce::ComboBox algoNegBox;
    juce::ComboBox blendModeBox;
    juce::ComboBox stereoModeBox;

    juce::Slider stagesSlider;
    juce::Label sectionLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> algoPosAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> algoNegAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> blendModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> stereoModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stagesAttachment;
};

}
