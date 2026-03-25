#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"
#include "ModDragSource.h"

namespace gvnr {

class ModulationPanel : public juce::Component {
public:
    ModulationPanel(juce::AudioProcessorValueTreeState& apvts);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix) {
        envAttackKnob.setModulationMatrix(matrix);
        envReleaseKnob.setModulationMatrix(matrix);
        lfoRateKnob.setModulationMatrix(matrix);
        lfoMorphKnob.setModulationMatrix(matrix);
        macro1Knob.setModulationMatrix(matrix);
        macro2Knob.setModulationMatrix(matrix);
    }

private:
    KnobComponent envAttackKnob;
    KnobComponent envReleaseKnob;
    KnobComponent lfoRateKnob;
    KnobComponent lfoMorphKnob;
    KnobComponent macro1Knob;
    KnobComponent macro2Knob;

    ModDragSource envDragHandle  {ModSource::EnvFollower, "ENV"};
    ModDragSource lfoDragHandle  {ModSource::LFO, "LFO"};
    ModDragSource macro1DragHandle {ModSource::Macro1, "M1"};
    ModDragSource macro2DragHandle {ModSource::Macro2, "M2"};

    juce::ComboBox lfoShapeBox;
    juce::ToggleButton lfoSyncButton;

    juce::Label sectionLabel;
    juce::Label envLabel;
    juce::Label lfoLabel;
    juce::Label macroLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> lfoShapeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lfoSyncAttachment;
};

}
