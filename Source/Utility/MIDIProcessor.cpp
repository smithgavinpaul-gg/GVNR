#include "MIDIProcessor.h"

namespace gvnr {

void MIDIProcessor::prepare(double sr)
{
    sampleRate = sr;
}

void MIDIProcessor::process(const juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages) {
        auto message = metadata.getMessage();

        if (message.isNoteOn()) {
            currentNote = message.getNoteNumber();
            currentFrequency = static_cast<float>(message.getMidiNoteInHertz(currentNote));
            noteOn = true;
        } else if (message.isNoteOff()) {
            if (message.getNoteNumber() == currentNote) {
                noteOn = false;
            }
        }
    }
}

}
