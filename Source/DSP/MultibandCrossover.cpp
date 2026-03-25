#include "MultibandCrossover.h"

namespace gvnr {

void MultibandCrossover::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    currentChannels = numChannels;

    juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlock),
                                static_cast<juce::uint32>(numChannels)};

    lowCrossoverLP.prepare(spec);
    lowCrossoverHP.prepare(spec);
    highCrossoverLP.prepare(spec);
    highCrossoverHP.prepare(spec);

    lowCrossoverLP.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    lowCrossoverHP.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    highCrossoverLP.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    highCrossoverHP.setType(juce::dsp::LinkwitzRileyFilterType::highpass);

    tempBuffer.setSize(numChannels, samplesPerBlock);
}

void MultibandCrossover::reset()
{
    lowCrossoverLP.reset();
    lowCrossoverHP.reset();
    highCrossoverLP.reset();
    highCrossoverHP.reset();
}

void MultibandCrossover::split(const juce::AudioBuffer<float>& input, MultibandBuffers& bands,
                                float lowFreq, float highFreq)
{
    int numSamples = input.getNumSamples();
    int numChannels = input.getNumChannels();

    bands.low.setSize(numChannels, numSamples, false, false, true);
    bands.mid.setSize(numChannels, numSamples, false, false, true);
    bands.high.setSize(numChannels, numSamples, false, false, true);

    lowCrossoverLP.setCutoffFrequency(lowFreq);
    lowCrossoverHP.setCutoffFrequency(lowFreq);
    highCrossoverLP.setCutoffFrequency(highFreq);
    highCrossoverHP.setCutoffFrequency(highFreq);

    // Low band
    bands.low.makeCopyOf(input);
    juce::dsp::AudioBlock<float> lowBlock(bands.low);
    juce::dsp::ProcessContextReplacing<float> lowCtx(lowBlock);
    lowCrossoverLP.process(lowCtx);

    // Mid + High (everything above low crossover)
    tempBuffer.makeCopyOf(input);
    juce::dsp::AudioBlock<float> tempBlock(tempBuffer);
    juce::dsp::ProcessContextReplacing<float> tempCtx(tempBlock);
    lowCrossoverHP.process(tempCtx);

    // Mid band (below high crossover)
    bands.mid.makeCopyOf(tempBuffer, true);
    juce::dsp::AudioBlock<float> midBlock(bands.mid);
    juce::dsp::ProcessContextReplacing<float> midCtx(midBlock);
    highCrossoverLP.process(midCtx);

    // High band (above high crossover)
    bands.high.makeCopyOf(tempBuffer, true);
    juce::dsp::AudioBlock<float> highBlock(bands.high);
    juce::dsp::ProcessContextReplacing<float> highCtx(highBlock);
    highCrossoverHP.process(highCtx);
}

void MultibandCrossover::merge(const MultibandBuffers& bands, juce::AudioBuffer<float>& output)
{
    int numSamples = output.getNumSamples();
    int numChannels = output.getNumChannels();

    output.clear();

    for (int ch = 0; ch < numChannels; ++ch) {
        auto* out = output.getWritePointer(ch);
        const auto* lo = bands.low.getReadPointer(ch);
        const auto* mi = bands.mid.getReadPointer(ch);
        const auto* hi = bands.high.getReadPointer(ch);

        for (int i = 0; i < numSamples; ++i)
            out[i] = lo[i] + mi[i] + hi[i];
    }
}

}
