#pragma once

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

namespace gvnr {

class ModulationMatrix;

class KnobComponent : public juce::Component,
                      public juce::DragAndDropTarget {
public:
    KnobComponent(juce::AudioProcessorValueTreeState& apvts,
                  const juce::String& paramId,
                  const juce::String& labelText);

    void resized() override;
    void paint(juce::Graphics& g) override;

    void setModulationMatrix(ModulationMatrix* matrix);
    const juce::String& getParamId() const { return parameterID; }

    bool isInterestedInDragSource(const SourceDetails& details) override;
    void itemDragEnter(const SourceDetails& details) override;
    void itemDragExit(const SourceDetails& details) override;
    void itemDropped(const SourceDetails& details) override;

    void mouseDown(const juce::MouseEvent& e) override;

private:
    juce::Slider knob;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    juce::String parameterID;
    ModulationMatrix* modMatrix = nullptr;
    bool dragHovering = false;
};

}
