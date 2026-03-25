#pragma once

#include <juce_dsp/juce_dsp.h>

namespace gvnr {

struct MultibandBuffers {
    juce::AudioBuffer<float> low;
    juce::AudioBuffer<float> mid;
    juce::AudioBuffer<float> high;
};

class MultibandCrossover {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void split(const juce::AudioBuffer<float>& input, MultibandBuffers& bands,
               float lowFreq, float highFreq);
    void merge(const MultibandBuffers& bands, juce::AudioBuffer<float>& output);

private:
    using Filter = juce::dsp::LinkwitzRileyFilter<float>;

    Filter lowCrossoverLP, lowCrossoverHP;
    Filter highCrossoverLP, highCrossoverHP;

    juce::AudioBuffer<float> tempBuffer;
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    int currentChannels = 2;
};

}
