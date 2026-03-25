#include "PitchSnap.h"
#include <algorithm>

namespace gvnr {

const std::array<PitchSnap::Scale, PitchSnap::NUM_SCALES> PitchSnap::scales = {{
    {"Chromatic",       {true, true, true, true, true, true, true, true, true, true, true, true}},
    {"Major",           {true, false, true, false, true, true, false, true, false, true, false, true}},
    {"Minor",           {true, false, true, true, false, true, false, true, true, false, true, false}},
    {"Pentatonic Maj",  {true, false, true, false, true, false, false, true, false, true, false, false}},
    {"Pentatonic Min",  {true, false, false, true, false, true, false, true, false, false, true, false}},
    {"Blues",            {true, false, false, true, false, true, true, true, false, false, true, false}},
    {"Dorian",          {true, false, true, true, false, true, false, true, false, true, true, false}},
    {"Mixolydian",      {true, false, true, false, true, true, false, true, false, true, true, false}},
    {"Harmonic Min",    {true, false, true, true, false, true, false, true, true, false, false, true}},
    {"Whole Tone",      {true, false, true, false, true, false, true, false, true, false, true, false}}
}};

float PitchSnap::noteToFrequency(int midiNote)
{
    return 440.0f * std::pow(2.0f, (static_cast<float>(midiNote) - 69.0f) / 12.0f);
}

int PitchSnap::frequencyToNearestNote(float freq)
{
    if (freq <= 0.0f) return 69;
    return static_cast<int>(std::round(69.0f + 12.0f * std::log2(freq / 440.0f)));
}

float PitchSnap::snapToNearestNote(float freq)
{
    int note = frequencyToNearestNote(freq);
    return noteToFrequency(note);
}

float PitchSnap::snapFrequency(float freq, int scaleIndex, int keyRoot)
{
    if (freq <= 0.0f) return freq;
    scaleIndex = std::clamp(scaleIndex, 0, NUM_SCALES - 1);
    keyRoot = std::clamp(keyRoot, 0, 11);

    int nearestNote = frequencyToNearestNote(freq);
    const auto& scale = scales[static_cast<size_t>(scaleIndex)];

    for (int offset = 0; offset <= 6; ++offset) {
        int noteUp = nearestNote + offset;
        int noteDown = nearestNote - offset;

        if (noteUp >= 0 && noteUp < 128 &&
            scale.intervals[static_cast<size_t>((noteUp - keyRoot + 12) % 12)])
            return noteToFrequency(noteUp);
        if (noteDown >= 0 && noteDown < 128 &&
            scale.intervals[static_cast<size_t>((noteDown - keyRoot + 12) % 12)])
            return noteToFrequency(noteDown);
    }

    return snapToNearestNote(freq);
}

const char* PitchSnap::getScaleName(int index)
{
    if (index >= 0 && index < NUM_SCALES)
        return scales[static_cast<size_t>(index)].name;
    return "Unknown";
}

const char* PitchSnap::getKeyName(int index)
{
    static const char* keyNames[] = {
        "C", "C#", "D", "D#", "E", "F",
        "F#", "G", "G#", "A", "A#", "B"
    };
    if (index >= 0 && index < NUM_KEYS)
        return keyNames[index];
    return "C";
}

}
