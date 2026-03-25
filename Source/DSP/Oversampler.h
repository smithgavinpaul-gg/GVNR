#pragma once

#include <functional>
#include <memory>
#include <juce_dsp/juce_dsp.h>

namespace gvnr {

class Oversampler {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void setOversamplingFactor(int factorIndex);

    void process(juce::AudioBuffer<float>& buffer,
                 std::function<void(juce::AudioBuffer<float>&)> processCallback);

    float getLatency() const;

private:
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;
    int currentFactor = 1;
    int numChannels = 2;
    int maxBlock = 512;
    double sampleRate = 44100.0;
};

}
