#pragma once

#include <array>
#include <atomic>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "DSP/SignalChain.h"
#include "Modulation/ModulationMatrix.h"
#include "Presets/PresetManager.h"
#include "Utility/MIDIProcessor.h"
#include "Utility/Parameters.h"

class TheGVNRProcessor : public juce::AudioProcessor
{
public:
    TheGVNRProcessor();
    ~TheGVNRProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    gvnr::SignalChain& getSignalChain() { return signalChain; }
    gvnr::ModulationMatrix& getModMatrix() { return modMatrix; }
    gvnr::PresetManager& getPresetManager() { return presetManager; }

    static constexpr int kWaveformBufferSize = 512;
    const float* getWaveformData() const { return waveformRing.data(); }
    const std::atomic<int>& getWaveformWritePosAtomic() const { return waveformWritePos; }

private:
    juce::AudioProcessorValueTreeState apvts;
    gvnr::SignalChain signalChain;
    gvnr::ModulationMatrix modMatrix;
    gvnr::MIDIProcessor midiProcessor;
    gvnr::PresetManager presetManager;

    std::array<float, kWaveformBufferSize> waveformRing{};
    std::atomic<int> waveformWritePos{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TheGVNRProcessor)
};
