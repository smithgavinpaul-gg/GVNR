#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "KnobComponent.h"
#include "WaveformDisplay.h"
#include "../Modulation/ModulationMatrix.h"
#include "BinaryData.h"

class TheGVNRProcessor;

namespace gvnr {

class PlayView : public juce::Component {
public:
    PlayView(TheGVNRProcessor& processor);
    void resized() override;
    void paint(juce::Graphics& g) override;

    WaveformDisplay& getWaveformDisplay() { return waveformDisplay; }
    void setModulationMatrix(ModulationMatrix* matrix);

private:
    KnobComponent driveKnob;
    KnobComponent dryWetKnob;
    KnobComponent macro1Knob;
    KnobComponent macro2Knob;
    KnobComponent inputKnob;
    KnobComponent outputKnob;

    WaveformDisplay waveformDisplay;

    juce::Image backgroundImg;
    juce::Rectangle<int> logoArea;

    juce::Label titleLabel;
    juce::Label presetLabel;
};

}
