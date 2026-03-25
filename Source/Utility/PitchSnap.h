#pragma once

#include <cmath>
#include <array>
#include <vector>

namespace gvnr {

class PitchSnap {
public:
    static float snapFrequency(float freq, int scaleIndex, int keyRoot = 0);
    static float snapToNearestNote(float freq);
    static float noteToFrequency(int midiNote);
    static int frequencyToNearestNote(float freq);

    static constexpr int NUM_SCALES = 10;
    static constexpr int NUM_KEYS = 12;
    static const char* getScaleName(int index);
    static const char* getKeyName(int index);

private:
    struct Scale {
        const char* name;
        std::array<bool, 12> intervals;
    };

    static const std::array<Scale, NUM_SCALES> scales;
};

}
