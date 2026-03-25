#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"
#include "BinaryData.h"

namespace gvnr {

class ModulationMatrix;

class FilterPanel : public juce::Component {
public:
    FilterPanel(juce::AudioProcessorValueTreeState& apvts, bool isPre);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix) {
        cutoffKnob.setModulationMatrix(matrix);
        resoKnob.setModulationMatrix(matrix);
        morphKnob.setModulationMatrix(matrix);
    }

private:
    KnobComponent cutoffKnob;
    KnobComponent resoKnob;
    KnobComponent morphKnob;

    bool isPreFilter;

    juce::ComboBox typeBox;
    juce::ToggleButton enableButton;
    juce::Image backgroundImg;
    juce::Rectangle<int> logoArea;

    juce::Label sectionLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
};

}
