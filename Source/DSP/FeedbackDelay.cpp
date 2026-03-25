#include "FeedbackDelay.h"
#include "../Utility/PitchSnap.h"
#include <algorithm>
#include <cmath>

namespace gvnr {

void FeedbackDelay::prepare(double sampleRate, int, int)
{
    currentSampleRate = sampleRate;
    reset();
}

void FeedbackDelay::reset()
{
    std::fill(std::begin(delayBufferL), std::end(delayBufferL), 0.0f);
    std::fill(std::begin(delayBufferR), std::end(delayBufferR), 0.0f);
    writePos = 0;
}

float FeedbackDelay::getDelayTimeMs(float timeMs, bool sync, int syncRateIndex, double bpm,
                                     bool pitchSnap, int scaleIndex, int keyRoot) const
{
    if (sync && bpm > 0.0) {
        static const float syncMultipliers[] = {
            0.0625f, 0.125f, 0.25f, 0.5f, 1.0f,
            0.0416667f, 0.0833333f, 0.166667f,
            0.09375f, 0.1875f, 0.375f
        };
        int idx = std::clamp(syncRateIndex, 0, 10);
        float beatsPerMs = static_cast<float>(bpm) / 60000.0f;
        timeMs = syncMultipliers[idx] / beatsPerMs;
    }

    if (pitchSnap && currentNoteFreq > 0.0f) {
        float snappedFreq = snapToScale(currentNoteFreq, scaleIndex, keyRoot);
        timeMs = 1000.0f / snappedFreq;
    }

    return std::clamp(timeMs, 0.1f, static_cast<float>(MAX_DELAY_SAMPLES) / static_cast<float>(currentSampleRate) * 1000.0f);
}

float FeedbackDelay::snapToScale(float freq, int scaleIndex, int keyRoot) const
{
    return PitchSnap::snapFrequency(freq, scaleIndex, keyRoot);
}

void FeedbackDelay::process(juce::AudioBuffer<float>& buffer,
                             float timeMs, float feedback, bool pingPong,
                             bool sync, int syncRateIndex, double bpm,
                             bool pitchSnap, int scaleIndex, int keyRoot)
{
    float actualTimeMs = getDelayTimeMs(timeMs, sync, syncRateIndex, bpm, pitchSnap, scaleIndex, keyRoot);
    int delaySamples = static_cast<int>(actualTimeMs * 0.001f * static_cast<float>(currentSampleRate));
    delaySamples = std::clamp(delaySamples, 1, MAX_DELAY_SAMPLES - 1);

    float fb = std::clamp(feedback / 100.0f, 0.0f, 0.98f);
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    float* dataL = numChannels > 0 ? buffer.getWritePointer(0) : nullptr;
    float* dataR = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < numSamples; ++i) {
        int readPos = (writePos - delaySamples + MAX_DELAY_SAMPLES) % MAX_DELAY_SAMPLES;

        float delayedL = delayBufferL[readPos];
        float delayedR = delayBufferR[readPos];

        float inputL = dataL ? dataL[i] : 0.0f;
        float inputR = dataR ? dataR[i] : 0.0f;

        if (pingPong) {
            delayBufferL[writePos] = inputL + fb * delayedR;
            delayBufferR[writePos] = inputR + fb * delayedL;
        } else {
            delayBufferL[writePos] = inputL + fb * delayedL;
            delayBufferR[writePos] = inputR + fb * delayedR;
        }

        if (dataL) dataL[i] = inputL + fb * delayedL;
        if (dataR) dataR[i] = inputR + fb * delayedR;

        writePos = (writePos + 1) % MAX_DELAY_SAMPLES;
    }
}

}
