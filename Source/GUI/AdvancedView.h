#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "DistortionPanel.h"
#include "FilterPanel.h"
#include "FeedbackPanel.h"
#include "MultibandPanel.h"
#include "ModulationPanel.h"
#include "WaveformDisplay.h"
#include "../Modulation/ModulationMatrix.h"

class TheGVNRProcessor;

namespace gvnr {

class AdvancedView : public juce::Component {
public:
    AdvancedView(TheGVNRProcessor& processor);
    void resized() override;
    void paint(juce::Graphics& g) override;

    WaveformDisplay& getWaveformDisplay() { return waveformDisplay; }
    void setModulationMatrix(ModulationMatrix* matrix);

private:
    DistortionPanel distortionPanel;
    FilterPanel preFilterPanel;
    FilterPanel postFilterPanel;
    FeedbackPanel feedbackPanel;
    MultibandPanel multibandPanel;
    ModulationPanel modulationPanel;
    WaveformDisplay waveformDisplay;

    juce::Label titleLabel;
};

}
