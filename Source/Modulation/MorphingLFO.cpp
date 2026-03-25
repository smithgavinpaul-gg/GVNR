#include "MorphingLFO.h"

namespace gvnr {

void MorphingLFO::prepare(double sr, int)
{
    sampleRate = sr;
    setRate(1.0f);
    reset();
}

void MorphingLFO::reset()
{
    phase = 0.0f;
    currentValue = 0.0f;
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    lastRandomValue = dist(rng);
    nextRandomValue = dist(rng);
}

void MorphingLFO::setRate(float hz)
{
    rateHz = std::max(hz, 0.001f);
    phaseIncrement = rateHz / static_cast<float>(sampleRate);
}

void MorphingLFO::setShape(int shapeIndex)
{
    currentShape = std::clamp(shapeIndex, 0, 5);
}

void MorphingLFO::setMorph(float amount)
{
    morphAmount = std::clamp(amount, 0.0f, 1.0f);
}

void MorphingLFO::setSync(bool synced, double)
{
    isSynced = synced;
}

float MorphingLFO::generateShape(int shape, float p) const
{
    constexpr float twoPi = 6.283185307f;

    switch (shape) {
        case 0: return std::sin(p * twoPi);
        case 1: return (p < 0.5f) ? (4.0f * p - 1.0f) : (3.0f - 4.0f * p);
        case 2: return 2.0f * p - 1.0f;
        case 3: return 1.0f - 2.0f * p;
        case 4: return (p < 0.5f) ? 1.0f : -1.0f;
        case 5: return lastRandomValue + (nextRandomValue - lastRandomValue) * p;
        default: return 0.0f;
    }
}

void MorphingLFO::process(int numSamples)
{
    for (int i = 0; i < numSamples; ++i) {
        float baseValue = generateShape(currentShape, phase);

        int nextShape = (currentShape + 1) % 6;
        float morphedValue = generateShape(nextShape, phase);
        currentValue = baseValue * (1.0f - morphAmount) + morphedValue * morphAmount;

        phase += phaseIncrement;
        if (phase >= 1.0f) {
            phase -= 1.0f;
            if (currentShape == 5) {
                lastRandomValue = nextRandomValue;
                std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
                nextRandomValue = dist(rng);
            }
        }
    }
}

}
