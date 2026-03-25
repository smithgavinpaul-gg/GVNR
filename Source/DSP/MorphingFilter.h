#pragma once

#include <juce_dsp/juce_dsp.h>

namespace gvnr {

enum class FilterType {
    LowPass12, LowPass24, HighPass12, HighPass24,
    BandPass, Notch, AllPass, Peak,
    LowShelf, HighShelf, CombPos, CombNeg,
    PhaserStage1, PhaserStage2, PhaserStage4, PhaserStage6,
    VowelA, VowelE, VowelI, VowelO,
    FormantShift, Resonator, MorphLP_HP, MorphLP_BP,
    NumTypes
};

class MorphingFilter {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void process(juce::AudioBuffer<float>& buffer,
                 FilterType type, float cutoff, float resonance, float morph);

private:
    void updateCoefficients(FilterType type, float cutoff, float resonance, float morph);

    juce::dsp::StateVariableTPTFilter<float> svfFilterL;
    juce::dsp::StateVariableTPTFilter<float> svfFilterR;

    juce::IIRFilter iirL, iirR;
    juce::IIRFilter iirL2, iirR2;

    float combBufferL[8192] = {};
    float combBufferR[8192] = {};
    int combWritePos = 0;

    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
};

}
