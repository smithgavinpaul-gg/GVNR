#pragma once

#include <juce_dsp/juce_dsp.h>
#include "DistortionAlgorithms.h"

namespace gvnr {

class MultiPolarDistortion {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void process(juce::AudioBuffer<float>& buffer,
                 DistortionType positiveAlgo,
                 DistortionType negativeAlgo,
                 float drive,
                 float blend,
                 bool hardBlend,
                 int stages);

private:
    float processSample(float sample,
                        DistortionType positiveAlgo,
                        DistortionType negativeAlgo,
                        float drive,
                        float blend,
                        bool hardBlend,
                        int stages);

    double currentSampleRate = 44100.0;
};

}
