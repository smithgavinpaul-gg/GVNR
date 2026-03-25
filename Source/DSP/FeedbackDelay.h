#pragma once

#include <juce_dsp/juce_dsp.h>

namespace gvnr {

class FeedbackDelay {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void process(juce::AudioBuffer<float>& buffer,
                 float timeMs, float feedback, bool pingPong,
                 bool sync, int syncRateIndex, double bpm,
                 bool pitchSnap, int scaleIndex, int keyRoot = 0);

    void setNoteFrequency(float freq) { currentNoteFreq = freq; }

private:
    float getDelayTimeMs(float timeMs, bool sync, int syncRateIndex, double bpm,
                         bool pitchSnap, int scaleIndex, int keyRoot) const;
    float snapToScale(float freq, int scaleIndex, int keyRoot) const;

    static constexpr int MAX_DELAY_SAMPLES = 192000;
    float delayBufferL[MAX_DELAY_SAMPLES] = {};
    float delayBufferR[MAX_DELAY_SAMPLES] = {};
    int writePos = 0;

    double currentSampleRate = 44100.0;
    float currentNoteFreq = 0.0f;
};

}
