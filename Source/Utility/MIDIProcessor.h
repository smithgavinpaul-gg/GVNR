#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace gvnr {

class MIDIProcessor {
public:
    void prepare(double sampleRate);
    void process(const juce::MidiBuffer& midiMessages);

    float getCurrentNoteFrequency() const { return currentFrequency; }
    int getCurrentNote() const { return currentNote; }
    bool isNoteOn() const { return noteOn; }

private:
    float currentFrequency = 0.0f;
    int currentNote = -1;
    bool noteOn = false;
    double sampleRate = 44100.0;
};

}
