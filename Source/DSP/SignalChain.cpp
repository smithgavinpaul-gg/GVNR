#include "SignalChain.h"
#include <cmath>

namespace gvnr {

static constexpr float kNoiseFloorRms = 1.0e-3f;

static float computeRms(const juce::AudioBuffer<float>& buf)
{
    float sum = 0.0f;
    int total = 0;
    for (int ch = 0; ch < buf.getNumChannels(); ++ch) {
        const auto* data = buf.getReadPointer(ch);
        for (int i = 0; i < buf.getNumSamples(); ++i)
            sum += data[i] * data[i];
        total += buf.getNumSamples();
    }
    return total > 0 ? std::sqrt(sum / static_cast<float>(total)) : 0.0f;
}

static void softLimit(juce::AudioBuffer<float>& buf)
{
    for (int ch = 0; ch < buf.getNumChannels(); ++ch) {
        auto* data = buf.getWritePointer(ch);
        for (int i = 0; i < buf.getNumSamples(); ++i)
            data[i] = std::tanh(data[i]);
    }
}

void SignalChain::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    currentSampleRate = sampleRate;

    distortion.prepare(sampleRate, samplesPerBlock, numChannels);
    preFilter.prepare(sampleRate, samplesPerBlock, numChannels);
    postFilter.prepare(sampleRate, samplesPerBlock, numChannels);
    feedbackDelay.prepare(sampleRate, samplesPerBlock, numChannels);
    crossover.prepare(sampleRate, samplesPerBlock, numChannels);
    oversampler.prepare(sampleRate, samplesPerBlock, numChannels);

    inputGainSmoothed.reset(sampleRate, 0.02);
    outputGainSmoothed.reset(sampleRate, 0.02);
    dryWetSmoothed.reset(sampleRate, 0.02);
}

void SignalChain::reset()
{
    distortion.reset();
    preFilter.reset();
    postFilter.reset();
    feedbackDelay.reset();
    crossover.reset();
    oversampler.reset();
}

void SignalChain::process(juce::AudioBuffer<float>& buffer,
                           juce::AudioProcessorValueTreeState& apvts,
                           ModulationMatrix& modMatrix,
                           MIDIProcessor& midiProcessor)
{
    auto inputGainDb = apvts.getRawParameterValue(ParamIDs::INPUT_GAIN)->load();
    auto outputGainDb = apvts.getRawParameterValue(ParamIDs::OUTPUT_GAIN)->load();
    auto dryWetPct = apvts.getRawParameterValue(ParamIDs::DRY_WET)->load();

    inputGainSmoothed.setTargetValue(juce::Decibels::decibelsToGain(inputGainDb));
    outputGainSmoothed.setTargetValue(juce::Decibels::decibelsToGain(outputGainDb));
    dryWetSmoothed.setTargetValue(dryWetPct / 100.0f);

    auto oversamplingIdx = static_cast<int>(apvts.getRawParameterValue(ParamIDs::OVERSAMPLING)->load());
    oversampler.setOversamplingFactor(oversamplingIdx);

    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    // Input gain
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
            data[i] *= inputGainSmoothed.getNextValue();
    }

    // MIDI pitch tracking for feedback
    float noteFreq = midiProcessor.getCurrentNoteFrequency();
    feedbackDelay.setNoteFrequency(noteFreq);

    auto distDrive = apvts.getRawParameterValue(ParamIDs::DIST_DRIVE)->load() / 100.0f;
    auto distAlgoPos = static_cast<DistortionType>(static_cast<int>(apvts.getRawParameterValue(ParamIDs::DIST_ALGO_POS)->load()));
    auto distAlgoNeg = static_cast<DistortionType>(static_cast<int>(apvts.getRawParameterValue(ParamIDs::DIST_ALGO_NEG)->load()));
    auto distBlend = apvts.getRawParameterValue(ParamIDs::DIST_BLEND)->load();
    auto distBlendMode = static_cast<int>(apvts.getRawParameterValue(ParamIDs::DIST_BLEND_MODE)->load());
    auto distStages = static_cast<int>(apvts.getRawParameterValue(ParamIDs::DIST_STAGES)->load());

    auto preFilterEnabled = apvts.getRawParameterValue(ParamIDs::FILTER_PRE_ENABLED)->load() > 0.5f;
    auto preFilterType = static_cast<FilterType>(static_cast<int>(apvts.getRawParameterValue(ParamIDs::FILTER_PRE_TYPE)->load()));
    auto preFilterCutoff = apvts.getRawParameterValue(ParamIDs::FILTER_PRE_CUTOFF)->load();
    auto preFilterReso = apvts.getRawParameterValue(ParamIDs::FILTER_PRE_RESO)->load();
    auto preFilterMorph = apvts.getRawParameterValue(ParamIDs::FILTER_PRE_MORPH)->load();

    auto postFilterEnabled = apvts.getRawParameterValue(ParamIDs::FILTER_POST_ENABLED)->load() > 0.5f;
    auto postFilterType = static_cast<FilterType>(static_cast<int>(apvts.getRawParameterValue(ParamIDs::FILTER_POST_TYPE)->load()));
    auto postFilterCutoff = apvts.getRawParameterValue(ParamIDs::FILTER_POST_CUTOFF)->load();
    auto postFilterReso = apvts.getRawParameterValue(ParamIDs::FILTER_POST_RESO)->load();
    auto postFilterMorph = apvts.getRawParameterValue(ParamIDs::FILTER_POST_MORPH)->load();

    auto fbEnabled = apvts.getRawParameterValue(ParamIDs::FB_ENABLED)->load() > 0.5f;
    auto fbTime = apvts.getRawParameterValue(ParamIDs::FB_TIME)->load();
    auto fbFeedback = apvts.getRawParameterValue(ParamIDs::FB_FEEDBACK)->load();
    auto fbPingPong = apvts.getRawParameterValue(ParamIDs::FB_PING_PONG)->load() > 0.5f;
    auto fbSync = apvts.getRawParameterValue(ParamIDs::FB_SYNC)->load() > 0.5f;
    auto fbSyncRate = static_cast<int>(apvts.getRawParameterValue(ParamIDs::FB_SYNC_RATE)->load());
    auto fbPitchSnap = apvts.getRawParameterValue(ParamIDs::FB_PITCH_SNAP)->load() > 0.5f;
    auto fbScale = static_cast<int>(apvts.getRawParameterValue(ParamIDs::FB_SCALE)->load());
    auto fbKey = static_cast<int>(apvts.getRawParameterValue(ParamIDs::FB_KEY)->load());

    auto mbEnabled = apvts.getRawParameterValue(ParamIDs::MB_ENABLED)->load() > 0.5f;

    bool inputIsSilent = computeRms(buffer) < kNoiseFloorRms;

    if (!inputIsSilent) {
        oversampler.process(buffer, [&](juce::AudioBuffer<float>& procBuffer) {
            if (preFilterEnabled)
                preFilter.process(procBuffer, preFilterType, preFilterCutoff, preFilterReso, preFilterMorph);

            if (mbEnabled) {
                auto mbLowFreq = apvts.getRawParameterValue(ParamIDs::MB_LOW_FREQ)->load();
                auto mbHighFreq = apvts.getRawParameterValue(ParamIDs::MB_HIGH_FREQ)->load();
                auto mbLowDist = apvts.getRawParameterValue(ParamIDs::MB_LOW_DIST)->load() > 0.5f;
                auto mbMidDist = apvts.getRawParameterValue(ParamIDs::MB_MID_DIST)->load() > 0.5f;
                auto mbHighDist = apvts.getRawParameterValue(ParamIDs::MB_HIGH_DIST)->load() > 0.5f;

                MultibandBuffers bands;
                crossover.split(procBuffer, bands, mbLowFreq, mbHighFreq);

                if (mbLowDist) distortion.process(bands.low, distAlgoPos, distAlgoNeg, distDrive, distBlend, distBlendMode == 1, distStages);
                if (mbMidDist) distortion.process(bands.mid, distAlgoPos, distAlgoNeg, distDrive, distBlend, distBlendMode == 1, distStages);
                if (mbHighDist) distortion.process(bands.high, distAlgoPos, distAlgoNeg, distDrive, distBlend, distBlendMode == 1, distStages);

                crossover.merge(bands, procBuffer);
            } else {
                distortion.process(procBuffer, distAlgoPos, distAlgoNeg, distDrive, distBlend, distBlendMode == 1, distStages);
            }

            if (fbEnabled)
                feedbackDelay.process(procBuffer, fbTime, fbFeedback, fbPingPong, fbSync, fbSyncRate, 120.0, fbPitchSnap, fbScale, fbKey);

            if (postFilterEnabled)
                postFilter.process(procBuffer, postFilterType, postFilterCutoff, postFilterReso, postFilterMorph);

            softLimit(procBuffer);
        });
    }

    // Dry/wet mix and output gain
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* wet = buffer.getWritePointer(ch);
        const auto* dry = dryBuffer.getReadPointer(ch);

        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float dw = dryWetSmoothed.getNextValue();
            wet[i] = dry[i] * (1.0f - dw) + wet[i] * dw;
            wet[i] *= outputGainSmoothed.getNextValue();
        }
    }
}

}
