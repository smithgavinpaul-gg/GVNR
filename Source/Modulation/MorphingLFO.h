#pragma once

#include <cmath>
#include <random>
#include <algorithm>

namespace gvnr {

class MorphingLFO {
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void process(int numSamples);

    void setRate(float hz);
    void setShape(int shapeIndex);
    void setMorph(float morphAmount);
    void setSync(bool synced, double bpm = 120.0);

    float getCurrentValue() const { return currentValue; }

private:
    float generateShape(int shape, float phase) const;

    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float currentValue = 0.0f;
    float rateHz = 1.0f;
    int currentShape = 0;
    float morphAmount = 0.0f;
    bool isSynced = false;
    float lastRandomValue = 0.0f;
    float nextRandomValue = 0.0f;

    double sampleRate = 44100.0;
    std::mt19937 rng{std::random_device{}()};
};

}
