#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MultiPolarDistortion.h"
#include "MorphingFilter.h"
#include "FeedbackDelay.h"
#include "MultibandCrossover.h"
#include "Oversampler.h"
#include "../Modulation/ModulationMatrix.h"
#include "../Utility/MIDIProcessor.h"
#include "../Utility/Parameters.h"

namespace gvnr {

class SignalChain {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void reset();
    void process(juce::AudioBuffer<float>& buffer,
                 juce::AudioProcessorValueTreeState& apvts,
                 ModulationMatrix& modMatrix,
                 MIDIProcessor& midiProcessor);

    MultiPolarDistortion& getDistortion() { return distortion; }
    MorphingFilter& getPreFilter() { return preFilter; }
    MorphingFilter& getPostFilter() { return postFilter; }
    FeedbackDelay& getFeedbackDelay() { return feedbackDelay; }

private:
    MultiPolarDistortion distortion;
    MorphingFilter preFilter;
    MorphingFilter postFilter;
    FeedbackDelay feedbackDelay;
    MultibandCrossover crossover;
    Oversampler oversampler;

    juce::SmoothedValue<float> inputGainSmoothed;
    juce::SmoothedValue<float> outputGainSmoothed;
    juce::SmoothedValue<float> dryWetSmoothed;

    double currentSampleRate = 44100.0;
};

}
