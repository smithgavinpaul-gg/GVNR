#include "Oversampler.h"
#include <algorithm>

namespace gvnr {

void Oversampler::prepare(double sr, int samplesPerBlock, int channels)
{
    sampleRate = sr;
    maxBlock = samplesPerBlock;
    numChannels = channels;
    setOversamplingFactor(currentFactor);
}

void Oversampler::reset()
{
    if (oversampling)
        oversampling->reset();
}

void Oversampler::setOversamplingFactor(int factorIndex)
{
    currentFactor = std::clamp(factorIndex, 0, 3);

    oversampling = std::make_unique<juce::dsp::Oversampling<float>>(
        numChannels,
        currentFactor,
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
        true);

    oversampling->initProcessing(static_cast<size_t>(maxBlock));
}

void Oversampler::process(juce::AudioBuffer<float>& buffer,
                           std::function<void(juce::AudioBuffer<float>&)> processCallback)
{
    if (currentFactor == 0 || oversampling == nullptr) {
        processCallback(buffer);
        return;
    }

    juce::dsp::AudioBlock<float> block(buffer);
    auto oversampledBlock = oversampling->processSamplesUp(block);

    juce::AudioBuffer<float> tempBuffer(static_cast<int>(oversampledBlock.getNumChannels()),
                                         static_cast<int>(oversampledBlock.getNumSamples()));

    for (int ch = 0; ch < static_cast<int>(oversampledBlock.getNumChannels()); ++ch) {
        auto* src = oversampledBlock.getChannelPointer(static_cast<size_t>(ch));
        auto* dst = tempBuffer.getWritePointer(ch);
        std::copy(src, src + oversampledBlock.getNumSamples(), dst);
    }

    processCallback(tempBuffer);

    for (int ch = 0; ch < static_cast<int>(oversampledBlock.getNumChannels()); ++ch) {
        auto* src = tempBuffer.getReadPointer(ch);
        auto* dst = oversampledBlock.getChannelPointer(static_cast<size_t>(ch));
        std::copy(src, src + oversampledBlock.getNumSamples(), dst);
    }

    oversampling->processSamplesDown(block);
}

float Oversampler::getLatency() const
{
    if (oversampling)
        return static_cast<float>(oversampling->getLatencyInSamples());
    return 0.0f;
}

}
