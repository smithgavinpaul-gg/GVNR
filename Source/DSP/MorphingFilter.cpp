#include "MorphingFilter.h"
#include <algorithm>

namespace gvnr {

void MorphingFilter::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;

    juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlock),
                                static_cast<juce::uint32>(numChannels)};

    svfFilterL.prepare(spec);
    svfFilterR.prepare(spec);
    svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

    iirL.reset();
    iirR.reset();
    iirL2.reset();
    iirR2.reset();

    std::fill(std::begin(combBufferL), std::end(combBufferL), 0.0f);
    std::fill(std::begin(combBufferR), std::end(combBufferR), 0.0f);
    combWritePos = 0;
}

void MorphingFilter::reset()
{
    svfFilterL.reset();
    svfFilterR.reset();
    iirL.reset();
    iirR.reset();
    iirL2.reset();
    iirR2.reset();
    std::fill(std::begin(combBufferL), std::end(combBufferL), 0.0f);
    std::fill(std::begin(combBufferR), std::end(combBufferR), 0.0f);
    combWritePos = 0;
}

void MorphingFilter::updateCoefficients(FilterType type, float cutoff, float resonance, float morph)
{
    cutoff = std::clamp(cutoff, 20.0f, static_cast<float>(currentSampleRate * 0.49));
    resonance = std::clamp(resonance, 0.0f, 100.0f);
    float q = 0.5f + resonance / 100.0f * 9.5f;

    switch (type) {
        case FilterType::LowPass12:
        case FilterType::LowPass24:
            svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            break;

        case FilterType::HighPass12:
        case FilterType::HighPass24:
            svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            break;

        case FilterType::BandPass:
            svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            break;

        case FilterType::Notch: {
            auto coeffs = juce::IIRCoefficients::makeNotchFilter(currentSampleRate, cutoff, q);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            break;
        }

        case FilterType::AllPass: {
            auto coeffs = juce::IIRCoefficients::makeAllPass(currentSampleRate, cutoff, q);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            break;
        }

        case FilterType::Peak: {
            float gain = 1.0f + morph / 100.0f * 24.0f;
            auto coeffs = juce::IIRCoefficients::makePeakFilter(currentSampleRate, cutoff, q, gain);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            break;
        }

        case FilterType::LowShelf: {
            float gain = 1.0f + morph / 100.0f * 12.0f;
            auto coeffs = juce::IIRCoefficients::makeLowShelf(currentSampleRate, cutoff, q, gain);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            break;
        }

        case FilterType::HighShelf: {
            float gain = 1.0f + morph / 100.0f * 12.0f;
            auto coeffs = juce::IIRCoefficients::makeHighShelf(currentSampleRate, cutoff, q, gain);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            break;
        }

        case FilterType::MorphLP_HP: {
            float lpAmount = 1.0f - morph / 100.0f;
            float hpAmount = morph / 100.0f;
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            (void)lpAmount; (void)hpAmount;
            break;
        }

        case FilterType::MorphLP_BP: {
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            break;
        }

        default:
            svfFilterL.setCutoffFrequency(cutoff);
            svfFilterR.setCutoffFrequency(cutoff);
            svfFilterL.setResonance(q);
            svfFilterR.setResonance(q);
            break;
    }
}

void MorphingFilter::process(juce::AudioBuffer<float>& buffer,
                              FilterType type, float cutoff, float resonance, float morph)
{
    updateCoefficients(type, cutoff, resonance, morph);
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    bool useSVF = (type <= FilterType::BandPass || type >= FilterType::MorphLP_HP);
    bool useIIR = (type >= FilterType::Notch && type <= FilterType::HighShelf);
    bool useComb = (type == FilterType::CombPos || type == FilterType::CombNeg);

    if (useComb) {
        int delaySamples = std::max(1, static_cast<int>(currentSampleRate / cutoff));
        delaySamples = std::min(delaySamples, 8191);
        float fb = resonance / 100.0f * 0.95f;
        float sign = (type == FilterType::CombNeg) ? -1.0f : 1.0f;

        for (int i = 0; i < numSamples; ++i) {
            int readPos = (combWritePos - delaySamples + 8192) % 8192;

            if (numChannels > 0) {
                float* dataL = buffer.getWritePointer(0);
                float delayed = combBufferL[readPos];
                float out = dataL[i] + sign * fb * delayed;
                combBufferL[combWritePos] = out;
                dataL[i] = out;
            }
            if (numChannels > 1) {
                float* dataR = buffer.getWritePointer(1);
                float delayed = combBufferR[readPos];
                float out = dataR[i] + sign * fb * delayed;
                combBufferR[combWritePos] = out;
                dataR[i] = out;
            }
            combWritePos = (combWritePos + 1) % 8192;
        }
        return;
    }

    if (useIIR) {
        if (numChannels > 0) iirL.processSamples(buffer.getWritePointer(0), numSamples);
        if (numChannels > 1) iirR.processSamples(buffer.getWritePointer(1), numSamples);
        return;
    }

    if (useSVF) {
        if (type == FilterType::MorphLP_HP) {
            float morphAmt = morph / 100.0f;
            for (int i = 0; i < numSamples; ++i) {
                if (numChannels > 0) {
                    float* dataL = buffer.getWritePointer(0);
                    svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
                    float lp = svfFilterL.processSample(0, dataL[i]);
                    svfFilterL.setType(juce::dsp::StateVariableTPTFilterType::highpass);
                    float hp = svfFilterL.processSample(0, dataL[i]);
                    dataL[i] = lp * (1.0f - morphAmt) + hp * morphAmt;
                }
                if (numChannels > 1) {
                    float* dataR = buffer.getWritePointer(1);
                    svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
                    float lp = svfFilterR.processSample(1, dataR[i]);
                    svfFilterR.setType(juce::dsp::StateVariableTPTFilterType::highpass);
                    float hp = svfFilterR.processSample(1, dataR[i]);
                    dataR[i] = lp * (1.0f - morphAmt) + hp * morphAmt;
                }
            }
        } else {
            for (int i = 0; i < numSamples; ++i) {
                if (numChannels > 0) {
                    float* dataL = buffer.getWritePointer(0);
                    dataL[i] = svfFilterL.processSample(0, dataL[i]);
                }
                if (numChannels > 1) {
                    float* dataR = buffer.getWritePointer(1);
                    dataR[i] = svfFilterR.processSample(1, dataR[i]);
                }
            }

            if (type == FilterType::LowPass24 || type == FilterType::HighPass24) {
                auto coeffs = (type == FilterType::LowPass24)
                    ? juce::IIRCoefficients::makeLowPass(currentSampleRate, cutoff)
                    : juce::IIRCoefficients::makeHighPass(currentSampleRate, cutoff);
                iirL2.setCoefficients(coeffs);
                iirR2.setCoefficients(coeffs);
                if (numChannels > 0) iirL2.processSamples(buffer.getWritePointer(0), numSamples);
                if (numChannels > 1) iirR2.processSamples(buffer.getWritePointer(1), numSamples);
            }
        }
        return;
    }

    // Phaser, vowel, formant, resonator all handled via allpass chain or bandpass pairs
    if (type >= FilterType::PhaserStage1 && type <= FilterType::PhaserStage6) {
        int stages = 1;
        if (type == FilterType::PhaserStage2) stages = 2;
        else if (type == FilterType::PhaserStage4) stages = 4;
        else if (type == FilterType::PhaserStage6) stages = 6;

        for (int s = 0; s < stages; ++s) {
            auto coeffs = juce::IIRCoefficients::makeAllPass(currentSampleRate, cutoff);
            iirL.setCoefficients(coeffs);
            iirR.setCoefficients(coeffs);
            if (numChannels > 0) iirL.processSamples(buffer.getWritePointer(0), numSamples);
            if (numChannels > 1) iirR.processSamples(buffer.getWritePointer(1), numSamples);
        }
        return;
    }

    if (type >= FilterType::VowelA && type <= FilterType::VowelO) {
        float f1 = 800.0f, f2 = 1200.0f;
        switch (type) {
            case FilterType::VowelA: f1 = 800.0f;  f2 = 1150.0f; break;
            case FilterType::VowelE: f1 = 400.0f;  f2 = 1600.0f; break;
            case FilterType::VowelI: f1 = 270.0f;  f2 = 2290.0f; break;
            case FilterType::VowelO: f1 = 520.0f;  f2 = 950.0f;  break;
            default: break;
        }
        float q = 3.0f + resonance / 100.0f * 12.0f;
        auto c1 = juce::IIRCoefficients::makeBandPass(currentSampleRate, f1, q);
        auto c2 = juce::IIRCoefficients::makeBandPass(currentSampleRate, f2, q);
        iirL.setCoefficients(c1);
        iirR.setCoefficients(c1);
        iirL2.setCoefficients(c2);
        iirR2.setCoefficients(c2);

        juce::AudioBuffer<float> temp(buffer);
        if (numChannels > 0) {
            iirL.processSamples(buffer.getWritePointer(0), numSamples);
            iirL2.processSamples(temp.getWritePointer(0), numSamples);
            auto* out = buffer.getWritePointer(0);
            auto* t = temp.getReadPointer(0);
            for (int i = 0; i < numSamples; ++i) out[i] = (out[i] + t[i]) * 0.5f;
        }
        if (numChannels > 1) {
            iirR.processSamples(buffer.getWritePointer(1), numSamples);
            iirR2.processSamples(temp.getWritePointer(1), numSamples);
            auto* out = buffer.getWritePointer(1);
            auto* t = temp.getReadPointer(1);
            for (int i = 0; i < numSamples; ++i) out[i] = (out[i] + t[i]) * 0.5f;
        }
        return;
    }

    // FormantShift and Resonator: use peak filters
    {
        float q = 3.0f + resonance / 100.0f * 12.0f;
        float gain = 1.0f + morph / 100.0f * 12.0f;
        auto coeffs = juce::IIRCoefficients::makePeakFilter(currentSampleRate, cutoff, q, gain);
        iirL.setCoefficients(coeffs);
        iirR.setCoefficients(coeffs);
        if (numChannels > 0) iirL.processSamples(buffer.getWritePointer(0), numSamples);
        if (numChannels > 1) iirR.processSamples(buffer.getWritePointer(1), numSamples);
    }
}

}
