#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Modulation/CurveSequencer.h"

namespace gvnr {

class CurveEditor : public juce::Component {
public:
    CurveEditor(CurveSequencer& sequencer);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;

private:
    juce::Point<float> pointToScreen(const CurvePoint& p) const;
    CurvePoint screenToPoint(juce::Point<float> screen) const;
    int findNearestPoint(juce::Point<float> screen) const;

    CurveSequencer& sequencer;
    int dragIndex = -1;
};

}
