#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Modulation/ModulationMatrix.h"

namespace gvnr {

class ModDragSource : public juce::Component {
public:
    ModDragSource(ModSource source, const juce::String& label);

    void paint(juce::Graphics& g) override;
    void mouseDrag(const juce::MouseEvent& e) override;

private:
    ModSource modSource;
    juce::String sourceLabel;
};

}
