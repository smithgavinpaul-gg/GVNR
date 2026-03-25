#pragma once

#include <cmath>
#include <algorithm>

namespace gvnr {

class EnvelopeFollower {
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void process(const float* input, int numSamples);

    void setAttack(float ms);
    void setRelease(float ms);

    float getCurrentValue() const { return currentValue; }

private:
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;
    float currentValue = 0.0f;
    double sampleRate = 44100.0;
};

}
