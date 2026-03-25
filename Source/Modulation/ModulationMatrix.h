#pragma once

#include <juce_core/juce_core.h>
#include "EnvelopeFollower.h"
#include "MorphingLFO.h"
#include "CurveSequencer.h"
#include "MacroControl.h"
#include <vector>
#include <string>

namespace gvnr {

enum class ModSource {
    EnvFollower,
    LFO,
    CurveSeq1,
    CurveSeq2,
    Macro1,
    Macro2,
    NumSources
};

struct ModRoute {
    ModSource source = ModSource::LFO;
    std::string targetParamId;
    float amount = 0.0f;
    bool bypassed = false;
    float skew = 0.0f;
};

class ModulationMatrix {
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void process(int numSamples);

    void addRoute(const ModRoute& route);
    void removeRoute(int index);
    void clearRoutes();
    const std::vector<ModRoute>& getRoutes() const { return routes; }

    float getModValue(ModSource source) const;
    float getModulatedValue(const std::string& paramId, float baseValue) const;

    EnvelopeFollower& getEnvFollower() { return envFollower; }
    MorphingLFO& getLFO() { return lfo; }
    CurveSequencer& getCurveSeq1() { return curveSeq1; }
    CurveSequencer& getCurveSeq2() { return curveSeq2; }
    MacroControl& getMacro1() { return macro1; }
    MacroControl& getMacro2() { return macro2; }

    void feedEnvelope(const float* input, int numSamples);

private:
    EnvelopeFollower envFollower;
    MorphingLFO lfo;
    CurveSequencer curveSeq1;
    CurveSequencer curveSeq2;
    MacroControl macro1;
    MacroControl macro2;

    std::vector<ModRoute> routes;
};

}
