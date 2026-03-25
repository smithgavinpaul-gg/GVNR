#include "ModulationMatrix.h"
#include <cmath>

namespace gvnr {

void ModulationMatrix::prepare(double sampleRate, int samplesPerBlock)
{
    envFollower.prepare(sampleRate, samplesPerBlock);
    lfo.prepare(sampleRate, samplesPerBlock);
    curveSeq1.prepare(sampleRate, samplesPerBlock);
    curveSeq2.prepare(sampleRate, samplesPerBlock);
}

void ModulationMatrix::reset()
{
    envFollower.reset();
    lfo.reset();
    curveSeq1.reset();
    curveSeq2.reset();
}

void ModulationMatrix::process(int numSamples)
{
    lfo.process(numSamples);
    curveSeq1.process(numSamples);
    curveSeq2.process(numSamples);
}

void ModulationMatrix::feedEnvelope(const float* input, int numSamples)
{
    envFollower.process(input, numSamples);
}

void ModulationMatrix::addRoute(const ModRoute& route)
{
    routes.push_back(route);
}

void ModulationMatrix::removeRoute(int index)
{
    if (index >= 0 && index < static_cast<int>(routes.size()))
        routes.erase(routes.begin() + index);
}

void ModulationMatrix::clearRoutes()
{
    routes.clear();
}

float ModulationMatrix::getModValue(ModSource source) const
{
    switch (source) {
        case ModSource::EnvFollower: return envFollower.getCurrentValue();
        case ModSource::LFO:        return lfo.getCurrentValue();
        case ModSource::CurveSeq1:  return curveSeq1.getCurrentValue();
        case ModSource::CurveSeq2:  return curveSeq2.getCurrentValue();
        case ModSource::Macro1:     return macro1.getValue();
        case ModSource::Macro2:     return macro2.getValue();
        default: return 0.0f;
    }
}

float ModulationMatrix::getModulatedValue(const std::string& paramId, float baseValue) const
{
    float totalMod = 0.0f;
    for (const auto& route : routes) {
        if (route.targetParamId == paramId && !route.bypassed) {
            float modVal = getModValue(route.source);
            if (std::abs(route.skew) > 0.001f) {
                float sign = (modVal >= 0.0f) ? 1.0f : -1.0f;
                modVal = sign * std::pow(std::abs(modVal), 1.0f + route.skew);
            }
            totalMod += modVal * route.amount;
        }
    }
    return baseValue + totalMod;
}

}
