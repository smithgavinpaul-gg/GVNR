#pragma once

#include <juce_core/juce_core.h>
#include <vector>

namespace gvnr {

struct CurvePoint {
    float x = 0.0f;
    float y = 0.0f;
    float curve = 0.0f;
};

class CurveSequencer {
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void process(int numSamples, double bpm = 120.0);

    void setRate(float hz);
    void setSync(bool synced);
    void setPoints(const std::vector<CurvePoint>& newPoints);

    float getCurrentValue() const { return currentValue; }
    const std::vector<CurvePoint>& getPoints() const { return points; }

    void addPoint(float x, float y, float curve = 0.0f);
    void removePoint(int index);
    void movePoint(int index, float x, float y);
    void clearPoints();
    void randomize();

private:
    float evaluateAtPhase(float phase) const;
    float interpolate(float y1, float y2, float t, float curve) const;

    std::vector<CurvePoint> points;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float currentValue = 0.0f;
    float rateHz = 1.0f;
    bool synced = false;
    double sampleRate = 44100.0;
};

}
