#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"

namespace gvnr {

class ModulationMatrix;

class MultibandPanel : public juce::Component {
public:
    MultibandPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix) {
        lowFreqKnob.setModulationMatrix(matrix);
        highFreqKnob.setModulationMatrix(matrix);
    }

private:
    juce::ToggleButton enableButton;
    KnobComponent lowFreqKnob;
    KnobComponent highFreqKnob;

    juce::ToggleButton lowDist, midDist, highDist;
    juce::ToggleButton lowFilter, midFilter, highFilter;
    juce::ToggleButton lowFb, midFb, highFb;

    juce::Label sectionLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowDistAtt, midDistAtt, highDistAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowFilterAtt, midFilterAtt, highFilterAtt;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowFbAtt, midFbAtt, highFbAtt;
};

}
