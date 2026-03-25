#include "MultiPolarDistortion.h"
#include <algorithm>

namespace gvnr {

void MultiPolarDistortion::prepare(double sampleRate, int, int)
{
    currentSampleRate = sampleRate;
}

void MultiPolarDistortion::reset() {}

void MultiPolarDistortion::process(juce::AudioBuffer<float>& buffer,
                                    DistortionType positiveAlgo,
                                    DistortionType negativeAlgo,
                                    float drive,
                                    float blend,
                                    bool hardBlend,
                                    int stages)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            data[i] = processSample(data[i], positiveAlgo, negativeAlgo,
                                     drive, blend, hardBlend, stages);
        }
    }
}

float MultiPolarDistortion::processSample(float sample,
                                           DistortionType positiveAlgo,
                                           DistortionType negativeAlgo,
                                           float drive,
                                           float blend,
                                           bool hardBlend,
                                           int stages)
{
    for (int s = 0; s < stages; ++s) {
        float positive = std::max(sample, 0.0f);
        float negative = std::min(sample, 0.0f);

        float processedPos = DistortionAlgorithms::process(positiveAlgo, positive, drive, 1.0f);
        float processedNeg = DistortionAlgorithms::process(negativeAlgo, negative, drive, 1.0f);

        float blendNorm = blend / 100.0f;

        if (hardBlend) {
            sample = (blendNorm >= 0.5f) ? processedPos + negative : positive + processedNeg;
        } else {
            float posContrib = positive * (1.0f - blendNorm) + processedPos * blendNorm;
            float negContrib = negative * blendNorm + processedNeg * (1.0f - blendNorm);
            sample = posContrib + negContrib;
        }
    }

    return std::clamp(sample, -1.0f, 1.0f);
}

}
