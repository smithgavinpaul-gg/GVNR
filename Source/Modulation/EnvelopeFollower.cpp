#include "EnvelopeFollower.h"

namespace gvnr {

void EnvelopeFollower::prepare(double sr, int)
{
    sampleRate = sr;
    setAttack(10.0f);
    setRelease(100.0f);
    currentValue = 0.0f;
}

void EnvelopeFollower::reset()
{
    currentValue = 0.0f;
}

void EnvelopeFollower::setAttack(float ms)
{
    float clampedMs = std::max(ms, 0.1f);
    attackCoeff = 1.0f - std::exp(-1.0f / (static_cast<float>(sampleRate) * clampedMs * 0.001f));
}

void EnvelopeFollower::setRelease(float ms)
{
    float clampedMs = std::max(ms, 1.0f);
    releaseCoeff = 1.0f - std::exp(-1.0f / (static_cast<float>(sampleRate) * clampedMs * 0.001f));
}

void EnvelopeFollower::process(const float* input, int numSamples)
{
    for (int i = 0; i < numSamples; ++i) {
        float rectified = std::abs(input[i]);
        float coeff = (rectified > currentValue) ? attackCoeff : releaseCoeff;
        currentValue += coeff * (rectified - currentValue);
    }
}

}
