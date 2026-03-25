#pragma once

#include <vector>
#include <utility>

namespace gvnr {

struct FactoryPresetData {
    const char* name;
    const char* category;
    std::vector<std::pair<const char*, float>> overrides;
};

namespace FactoryPresets {

inline std::vector<FactoryPresetData> getPresets()
{
    return {

        // =====================================================================
        // Category 1: Subtle / Warm
        // =====================================================================

        {"Warm Tube", "Subtle", {
            {"distDrive", 20.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"distBlend", 50.0f},
            {"filterPostCutoff", 8000.0f}, {"filterPostReso", 10.0f},
            {"oversampling", 2.0f}
        }},

        {"Gentle Tape", "Subtle", {
            {"distDrive", 15.0f}, {"distAlgoPos", 7.0f}, {"distAlgoNeg", 7.0f},
            {"filterPostCutoff", 10000.0f},
            {"outputGain", -1.0f}, {"oversampling", 2.0f}
        }},

        {"Analog Warmth", "Subtle", {
            {"distDrive", 30.0f}, {"distAlgoPos", 0.0f}, {"distAlgoNeg", 0.0f},
            {"filterPostCutoff", 7000.0f}, {"filterPostReso", 5.0f},
            {"dryWet", 80.0f}
        }},

        {"Silk", "Subtle", {
            {"distDrive", 10.0f}, {"distAlgoPos", 2.0f}, {"distAlgoNeg", 2.0f},
            {"filterPostCutoff", 12000.0f}, {"filterPostMorph", 20.0f},
            {"dryWet", 70.0f}
        }},

        {"Vintage Glow", "Subtle", {
            {"distDrive", 25.0f}, {"distAlgoPos", 3.0f}, {"distAlgoNeg", 0.0f},
            {"filterPreEnabled", 1.0f}, {"filterPreType", 0.0f},
            {"filterPreCutoff", 6000.0f},
            {"filterPostCutoff", 8000.0f}, {"outputGain", -2.0f}
        }},

        // =====================================================================
        // Category 2: Aggressive
        // =====================================================================

        {"Razorblade", "Aggressive", {
            {"distDrive", 95.0f}, {"distAlgoPos", 1.0f}, {"distAlgoNeg", 1.0f},
            {"distStages", 4.0f}, {"distBlendMode", 1.0f},
            {"filterPostCutoff", 6000.0f}, {"outputGain", -6.0f}
        }},

        {"Transistor Fury", "Aggressive", {
            {"distDrive", 85.0f}, {"distAlgoPos", 5.0f}, {"distAlgoNeg", 5.0f},
            {"distStages", 3.0f},
            {"filterPostCutoff", 4000.0f}, {"outputGain", -8.0f}
        }},

        {"Scorched Earth", "Aggressive", {
            {"distDrive", 100.0f}, {"distAlgoPos", 1.0f}, {"distAlgoNeg", 1.0f},
            {"distStages", 6.0f}, {"distBlendMode", 1.0f},
            {"filterPostCutoff", 3000.0f}, {"outputGain", -12.0f}
        }},

        {"Brutal", "Aggressive", {
            {"distDrive", 90.0f}, {"distAlgoPos", 6.0f}, {"distAlgoNeg", 6.0f},
            {"distStages", 2.0f}, {"distStereoMode", 2.0f},
            {"filterPostCutoff", 5000.0f}, {"outputGain", -8.0f}
        }},

        {"Fuzz Face", "Aggressive", {
            {"distDrive", 88.0f}, {"distAlgoPos", 5.0f}, {"distAlgoNeg", 3.0f},
            {"distStages", 3.0f}, {"distBlend", 70.0f},
            {"filterPostCutoff", 4500.0f}, {"filterPostReso", 15.0f},
            {"outputGain", -6.0f}
        }},

        // =====================================================================
        // Category 3: Bass Design
        // =====================================================================

        {"Sub Destroyer", "Bass", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 150.0f}, {"mbHighFreq", 3000.0f},
            {"mbLowDist", 1.0f}, {"mbMidDist", 0.0f}, {"mbHighDist", 0.0f},
            {"distDrive", 70.0f}, {"distAlgoPos", 8.0f}, {"distAlgoNeg", 8.0f},
            {"filterPostCutoff", 2000.0f}, {"outputGain", -4.0f}
        }},

        {"Low End Theory", "Bass", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 200.0f}, {"mbHighFreq", 4000.0f},
            {"mbLowDist", 1.0f}, {"mbMidDist", 0.0f}, {"mbHighDist", 0.0f},
            {"distDrive", 60.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"outputGain", -3.0f}
        }},

        {"Seismic", "Bass", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 100.0f}, {"mbHighFreq", 2000.0f},
            {"mbLowDist", 1.0f}, {"mbMidDist", 1.0f}, {"mbHighDist", 0.0f},
            {"distDrive", 80.0f}, {"distAlgoPos", 1.0f}, {"distAlgoNeg", 0.0f},
            {"distStages", 2.0f},
            {"filterPostCutoff", 3000.0f}, {"outputGain", -6.0f}
        }},

        {"Bass Face", "Bass", {
            {"distDrive", 65.0f}, {"distAlgoPos", 8.0f}, {"distAlgoNeg", 8.0f},
            {"filterPreEnabled", 1.0f}, {"filterPreType", 0.0f},
            {"filterPreCutoff", 400.0f},
            {"filterPostCutoff", 2500.0f}, {"outputGain", -5.0f}
        }},

        {"Deep Impact", "Bass", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 120.0f}, {"mbHighFreq", 3500.0f},
            {"mbLowDist", 1.0f}, {"mbMidDist", 0.0f}, {"mbHighDist", 0.0f},
            {"distDrive", 55.0f}, {"distAlgoPos", 7.0f}, {"distAlgoNeg", 7.0f},
            {"filterPostCutoff", 4000.0f}
        }},

        // =====================================================================
        // Category 4: Bitcrusher / Lo-Fi
        // =====================================================================

        {"8-Bit Dreams", "Lo-Fi", {
            {"distDrive", 60.0f}, {"distAlgoPos", 11.0f}, {"distAlgoNeg", 11.0f},
            {"filterPostCutoff", 8000.0f}, {"outputGain", -3.0f}
        }},

        {"Lo-Fi Crunch", "Lo-Fi", {
            {"distDrive", 45.0f}, {"distAlgoPos", 12.0f}, {"distAlgoNeg", 12.0f},
            {"filterPostCutoff", 6000.0f}, {"filterPostReso", 10.0f},
            {"dryWet", 85.0f}
        }},

        {"Retro Digital", "Lo-Fi", {
            {"distDrive", 70.0f}, {"distAlgoPos", 11.0f}, {"distAlgoNeg", 12.0f},
            {"distBlend", 60.0f}, {"distStages", 2.0f},
            {"filterPostCutoff", 5000.0f}, {"outputGain", -5.0f}
        }},

        {"Chiptune", "Lo-Fi", {
            {"distDrive", 85.0f}, {"distAlgoPos", 11.0f}, {"distAlgoNeg", 11.0f},
            {"distStages", 3.0f},
            {"filterPostCutoff", 4000.0f}, {"outputGain", -8.0f}
        }},

        {"VHS Warm", "Lo-Fi", {
            {"distDrive", 30.0f}, {"distAlgoPos", 7.0f}, {"distAlgoNeg", 11.0f},
            {"distBlend", 40.0f},
            {"filterPostCutoff", 7000.0f}, {"filterPostReso", 5.0f},
            {"dryWet", 75.0f}
        }},

        // =====================================================================
        // Category 5: Resonant Feedback
        // =====================================================================

        {"Resonance Machine", "Feedback", {
            {"fbEnabled", 1.0f}, {"fbTime", 50.0f}, {"fbFeedback", 65.0f},
            {"fbPitchSnap", 1.0f}, {"fbScale", 0.0f},
            {"distDrive", 40.0f}, {"distAlgoPos", 2.0f}, {"distAlgoNeg", 2.0f},
            {"filterPostCutoff", 8000.0f}, {"outputGain", -4.0f}
        }},

        {"Harmonic Feedback", "Feedback", {
            {"fbEnabled", 1.0f}, {"fbTime", 80.0f}, {"fbFeedback", 55.0f},
            {"fbPitchSnap", 1.0f}, {"fbScale", 1.0f},
            {"distDrive", 35.0f}, {"distAlgoPos", 0.0f}, {"distAlgoNeg", 0.0f},
            {"outputGain", -3.0f}
        }},

        {"Minor Key Echo", "Feedback", {
            {"fbEnabled", 1.0f}, {"fbTime", 120.0f}, {"fbFeedback", 60.0f},
            {"fbPitchSnap", 1.0f}, {"fbScale", 2.0f},
            {"distDrive", 30.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"filterPostCutoff", 6000.0f}
        }},

        {"Ping Pong Resonator", "Feedback", {
            {"fbEnabled", 1.0f}, {"fbTime", 150.0f}, {"fbFeedback", 50.0f},
            {"fbPingPong", 1.0f}, {"fbPitchSnap", 1.0f}, {"fbScale", 3.0f},
            {"distDrive", 25.0f}, {"distAlgoPos", 0.0f},
            {"dryWet", 80.0f}
        }},

        {"Blues Feedback", "Feedback", {
            {"fbEnabled", 1.0f}, {"fbTime", 100.0f}, {"fbFeedback", 70.0f},
            {"fbPitchSnap", 1.0f}, {"fbScale", 5.0f},
            {"distDrive", 45.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"filterPostCutoff", 5000.0f}, {"outputGain", -5.0f}
        }},

        // =====================================================================
        // Category 6: Filter Sweep
        // =====================================================================

        {"Auto Wah", "Filter", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 4.0f},
            {"filterPreCutoff", 800.0f}, {"filterPreReso", 60.0f},
            {"modLfoRate", 2.0f}, {"modLfoShape", 0.0f},
            {"distDrive", 30.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f}
        }},

        {"Phaser Dreams", "Filter", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 8.0f},
            {"filterPreCutoff", 1500.0f}, {"filterPreReso", 50.0f},
            {"filterPreMorph", 40.0f},
            {"filterPostReso", 30.0f}, {"filterPostMorph", 60.0f},
            {"modLfoRate", 0.5f}, {"modLfoShape", 1.0f},
            {"distDrive", 20.0f}
        }},

        {"Slow Sweep", "Filter", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 0.0f},
            {"filterPreCutoff", 2000.0f}, {"filterPreReso", 70.0f},
            {"filterPreMorph", 30.0f},
            {"modLfoRate", 0.1f}, {"modLfoShape", 1.0f},
            {"distDrive", 40.0f}, {"distAlgoPos", 2.0f}
        }},

        {"Rhythmic Filter", "Filter", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 0.0f},
            {"filterPreCutoff", 3000.0f}, {"filterPreReso", 55.0f},
            {"modLfoRate", 4.0f}, {"modLfoShape", 4.0f}, {"modLfoSync", 1.0f},
            {"distDrive", 35.0f}, {"distAlgoPos", 0.0f}
        }},

        {"Bandpass Motion", "Filter", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 4.0f},
            {"filterPreCutoff", 1200.0f}, {"filterPreReso", 65.0f},
            {"filterPreMorph", 50.0f},
            {"modLfoRate", 1.5f}, {"modLfoShape", 1.0f},
            {"distDrive", 25.0f}
        }},

        // =====================================================================
        // Category 7: Multiband Split
        // =====================================================================

        {"Three Way Split", "Multiband", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 250.0f}, {"mbHighFreq", 4000.0f},
            {"distDrive", 60.0f}, {"distAlgoPos", 0.0f}, {"distAlgoNeg", 4.0f},
            {"distBlend", 65.0f}
        }},

        {"Clean Bottom Dirty Top", "Multiband", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 300.0f}, {"mbHighFreq", 5000.0f},
            {"mbLowDist", 0.0f}, {"mbMidDist", 1.0f}, {"mbHighDist", 1.0f},
            {"distDrive", 75.0f}, {"distAlgoPos", 1.0f}, {"distAlgoNeg", 1.0f},
            {"outputGain", -4.0f}
        }},

        {"Mid Scoop Crush", "Multiband", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 200.0f}, {"mbHighFreq", 6000.0f},
            {"mbLowDist", 0.0f}, {"mbMidDist", 1.0f}, {"mbHighDist", 0.0f},
            {"distDrive", 80.0f}, {"distAlgoPos", 11.0f}, {"distAlgoNeg", 11.0f},
            {"outputGain", -5.0f}
        }},

        {"Hi-Fi Lows", "Multiband", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 250.0f}, {"mbHighFreq", 3000.0f},
            {"mbLowDist", 0.0f}, {"mbMidDist", 0.0f}, {"mbHighDist", 1.0f},
            {"distDrive", 65.0f}, {"distAlgoPos", 8.0f}, {"distAlgoNeg", 8.0f},
            {"filterPostCutoff", 8000.0f}
        }},

        {"Full Spectrum Chaos", "Multiband", {
            {"mbEnabled", 1.0f}, {"mbLowFreq", 180.0f}, {"mbHighFreq", 5500.0f},
            {"distDrive", 70.0f}, {"distAlgoPos", 8.0f}, {"distAlgoNeg", 11.0f},
            {"distStages", 3.0f}, {"distBlend", 30.0f},
            {"outputGain", -8.0f}
        }},

        // =====================================================================
        // Category 8: Creative / Experimental
        // =====================================================================

        {"Sine Folder", "Creative", {
            {"distDrive", 75.0f}, {"distAlgoPos", 9.0f}, {"distAlgoNeg", 9.0f},
            {"distStages", 6.0f}, {"distBlendMode", 1.0f},
            {"filterPostCutoff", 6000.0f}, {"outputGain", -10.0f}
        }},

        {"Alien", "Creative", {
            {"distDrive", 65.0f}, {"distAlgoPos", 10.0f}, {"distAlgoNeg", 8.0f},
            {"distStages", 4.0f}, {"distBlend", 70.0f},
            {"fbEnabled", 1.0f}, {"fbTime", 30.0f}, {"fbFeedback", 45.0f},
            {"filterPreEnabled", 1.0f}, {"filterPreType", 4.0f},
            {"filterPreReso", 80.0f}, {"filterPreMorph", 50.0f},
            {"modLfoRate", 3.0f}, {"modLfoShape", 5.0f},
            {"outputGain", -8.0f}
        }},

        {"Glitch Machine", "Creative", {
            {"distDrive", 80.0f}, {"distAlgoPos", 11.0f}, {"distAlgoNeg", 12.0f},
            {"distStages", 3.0f},
            {"fbEnabled", 1.0f}, {"fbTime", 40.0f}, {"fbFeedback", 55.0f},
            {"modLfoRate", 8.0f}, {"modLfoShape", 5.0f},
            {"outputGain", -10.0f}
        }},

        {"Textural Noise", "Creative", {
            {"distDrive", 55.0f}, {"distAlgoPos", 3.0f}, {"distAlgoNeg", 10.0f},
            {"distStages", 5.0f}, {"distBlend", 35.0f},
            {"filterPreEnabled", 1.0f}, {"filterPreType", 4.0f},
            {"filterPreCutoff", 2000.0f}, {"filterPreReso", 40.0f},
            {"filterPostCutoff", 4000.0f},
            {"outputGain", -6.0f}
        }},

        {"Dimension Shift", "Creative", {
            {"distDrive", 50.0f}, {"distAlgoPos", 9.0f}, {"distAlgoNeg", 3.0f},
            {"distStereoMode", 2.0f}, {"distStages", 3.0f},
            {"fbEnabled", 1.0f}, {"fbTime", 80.0f}, {"fbFeedback", 40.0f},
            {"fbPingPong", 1.0f},
            {"filterPostMorph", 60.0f},
            {"outputGain", -5.0f}
        }},

        // =====================================================================
        // Category 9: Vocal / Formant
        // =====================================================================

        {"Vowel A", "Vocal", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 18.0f},
            {"filterPreCutoff", 800.0f}, {"filterPreReso", 70.0f},
            {"filterPreMorph", 10.0f},
            {"distDrive", 25.0f}, {"distAlgoPos", 2.0f}, {"distAlgoNeg", 2.0f}
        }},

        {"Vowel E", "Vocal", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 19.0f},
            {"filterPreCutoff", 1200.0f}, {"filterPreReso", 65.0f},
            {"filterPreMorph", 30.0f},
            {"distDrive", 20.0f}, {"distAlgoPos", 0.0f}, {"distAlgoNeg", 0.0f}
        }},

        {"Vowel I", "Vocal", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 20.0f},
            {"filterPreCutoff", 2500.0f}, {"filterPreReso", 60.0f},
            {"filterPreMorph", 50.0f},
            {"distDrive", 22.0f}, {"distAlgoPos", 2.0f}, {"distAlgoNeg", 2.0f}
        }},

        {"Vowel O", "Vocal", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 21.0f},
            {"filterPreCutoff", 600.0f}, {"filterPreReso", 75.0f},
            {"filterPreMorph", 70.0f},
            {"distDrive", 28.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f}
        }},

        {"Talking Machine", "Vocal", {
            {"filterPreEnabled", 1.0f}, {"filterPreType", 18.0f},
            {"filterPreCutoff", 1000.0f}, {"filterPreReso", 80.0f},
            {"modLfoRate", 0.3f}, {"modLfoShape", 1.0f}, {"modLfoMorph", 50.0f},
            {"distDrive", 30.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"dryWet", 85.0f}
        }},

        // =====================================================================
        // Category 10: Classic
        // =====================================================================

        {"Marshall Stack", "Classic", {
            {"distDrive", 65.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"distStages", 2.0f},
            {"filterPreEnabled", 1.0f}, {"filterPreType", 4.0f},
            {"filterPreCutoff", 700.0f}, {"filterPreReso", 20.0f},
            {"filterPostType", 0.0f}, {"filterPostCutoff", 4000.0f},
            {"filterPostReso", 10.0f},
            {"outputGain", -4.0f}, {"oversampling", 2.0f}
        }},

        {"Neve Preamp", "Classic", {
            {"distDrive", 15.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 4.0f},
            {"filterPostCutoff", 15000.0f},
            {"oversampling", 2.0f}, {"outputGain", 1.0f}
        }},

        {"1176 Crunch", "Classic", {
            {"distDrive", 45.0f}, {"distAlgoPos", 5.0f}, {"distAlgoNeg", 5.0f},
            {"modEnvAttack", 1.0f}, {"modEnvRelease", 50.0f},
            {"filterPostCutoff", 8000.0f}, {"outputGain", -2.0f},
            {"oversampling", 2.0f}
        }},

        {"Space Echo", "Classic", {
            {"distDrive", 30.0f}, {"distAlgoPos", 7.0f}, {"distAlgoNeg", 7.0f},
            {"fbEnabled", 1.0f}, {"fbTime", 300.0f}, {"fbFeedback", 45.0f},
            {"filterPostCutoff", 6000.0f}, {"filterPostReso", 5.0f},
            {"dryWet", 80.0f}, {"outputGain", -2.0f}
        }},

        {"Fairchild", "Classic", {
            {"distDrive", 18.0f}, {"distAlgoPos", 4.0f}, {"distAlgoNeg", 0.0f},
            {"distBlend", 60.0f},
            {"filterPostCutoff", 12000.0f},
            {"modEnvAttack", 0.5f}, {"modEnvRelease", 200.0f},
            {"oversampling", 2.0f}, {"dryWet", 90.0f}
        }}
    };
}

} // namespace FactoryPresets
} // namespace gvnr
