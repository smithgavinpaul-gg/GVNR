#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace gvnr {

namespace ParamIDs {
    inline constexpr const char* INPUT_GAIN       = "inputGain";
    inline constexpr const char* OUTPUT_GAIN      = "outputGain";
    inline constexpr const char* DRY_WET          = "dryWet";
    inline constexpr const char* BYPASS           = "bypass";

    inline constexpr const char* DIST_DRIVE       = "distDrive";
    inline constexpr const char* DIST_ALGO_POS    = "distAlgoPos";
    inline constexpr const char* DIST_ALGO_NEG    = "distAlgoNeg";
    inline constexpr const char* DIST_BLEND       = "distBlend";
    inline constexpr const char* DIST_BLEND_MODE  = "distBlendMode";
    inline constexpr const char* DIST_STAGES      = "distStages";
    inline constexpr const char* DIST_STEREO_MODE = "distStereoMode";

    inline constexpr const char* FILTER_PRE_TYPE    = "filterPreType";
    inline constexpr const char* FILTER_PRE_CUTOFF  = "filterPreCutoff";
    inline constexpr const char* FILTER_PRE_RESO    = "filterPreReso";
    inline constexpr const char* FILTER_PRE_MORPH   = "filterPreMorph";
    inline constexpr const char* FILTER_PRE_ENABLED = "filterPreEnabled";

    inline constexpr const char* FILTER_POST_TYPE    = "filterPostType";
    inline constexpr const char* FILTER_POST_CUTOFF  = "filterPostCutoff";
    inline constexpr const char* FILTER_POST_RESO    = "filterPostReso";
    inline constexpr const char* FILTER_POST_MORPH   = "filterPostMorph";
    inline constexpr const char* FILTER_POST_ENABLED = "filterPostEnabled";

    inline constexpr const char* FB_TIME          = "fbTime";
    inline constexpr const char* FB_FEEDBACK      = "fbFeedback";
    inline constexpr const char* FB_PING_PONG     = "fbPingPong";
    inline constexpr const char* FB_SYNC          = "fbSync";
    inline constexpr const char* FB_SYNC_RATE     = "fbSyncRate";
    inline constexpr const char* FB_PITCH_SNAP    = "fbPitchSnap";
    inline constexpr const char* FB_SCALE         = "fbScale";
    inline constexpr const char* FB_KEY           = "fbKey";
    inline constexpr const char* FB_ENABLED       = "fbEnabled";

    inline constexpr const char* MB_ENABLED       = "mbEnabled";
    inline constexpr const char* MB_LOW_FREQ      = "mbLowFreq";
    inline constexpr const char* MB_HIGH_FREQ     = "mbHighFreq";
    inline constexpr const char* MB_LOW_DIST      = "mbLowDist";
    inline constexpr const char* MB_MID_DIST      = "mbMidDist";
    inline constexpr const char* MB_HIGH_DIST     = "mbHighDist";
    inline constexpr const char* MB_LOW_FILTER    = "mbLowFilter";
    inline constexpr const char* MB_MID_FILTER    = "mbMidFilter";
    inline constexpr const char* MB_HIGH_FILTER   = "mbHighFilter";
    inline constexpr const char* MB_LOW_FB        = "mbLowFb";
    inline constexpr const char* MB_MID_FB        = "mbMidFb";
    inline constexpr const char* MB_HIGH_FB       = "mbHighFb";

    inline constexpr const char* MOD_ENV_ATTACK   = "modEnvAttack";
    inline constexpr const char* MOD_ENV_RELEASE  = "modEnvRelease";
    inline constexpr const char* MOD_LFO_RATE     = "modLfoRate";
    inline constexpr const char* MOD_LFO_SHAPE    = "modLfoShape";
    inline constexpr const char* MOD_LFO_MORPH    = "modLfoMorph";
    inline constexpr const char* MOD_LFO_SYNC     = "modLfoSync";

    inline constexpr const char* MACRO_1          = "macro1";
    inline constexpr const char* MACRO_2          = "macro2";

    inline constexpr const char* OVERSAMPLING     = "oversampling";
}

inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::INPUT_GAIN, 1}, "Input Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::OUTPUT_GAIN, 1}, "Output Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::DRY_WET, 1}, "Dry/Wet",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::BYPASS, 1}, "Bypass", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::DIST_DRIVE, 1}, "Drive",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{ParamIDs::DIST_ALGO_POS, 1}, "Distortion Positive", 0, 29, 0));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{ParamIDs::DIST_ALGO_NEG, 1}, "Distortion Negative", 0, 29, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::DIST_BLEND, 1}, "Polar Blend",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::DIST_BLEND_MODE, 1}, "Blend Mode",
        juce::StringArray{"Smooth", "Hard"}, 0));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{ParamIDs::DIST_STAGES, 1}, "Distortion Stages", 1, 6, 1));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::DIST_STEREO_MODE, 1}, "Stereo Mode",
        juce::StringArray{"Stereo", "Mono", "Mid/Side"}, 0));

    auto freqRange = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f);

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{ParamIDs::FILTER_PRE_TYPE, 1}, "Pre-Filter Type", 0, 23, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_PRE_CUTOFF, 1}, "Pre-Filter Cutoff", freqRange, 1000.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_PRE_RESO, 1}, "Pre-Filter Resonance",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_PRE_MORPH, 1}, "Pre-Filter Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FILTER_PRE_ENABLED, 1}, "Pre-Filter Enabled", false));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{ParamIDs::FILTER_POST_TYPE, 1}, "Post-Filter Type", 0, 23, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_POST_CUTOFF, 1}, "Post-Filter Cutoff", freqRange, 5000.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_POST_RESO, 1}, "Post-Filter Resonance",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FILTER_POST_MORPH, 1}, "Post-Filter Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FILTER_POST_ENABLED, 1}, "Post-Filter Enabled", true));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FB_TIME, 1}, "Feedback Time",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.4f), 100.0f, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::FB_FEEDBACK, 1}, "Feedback Amount",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 30.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FB_PING_PONG, 1}, "Ping Pong", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FB_SYNC, 1}, "Feedback Sync", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::FB_SYNC_RATE, 1}, "Feedback Sync Rate",
        juce::StringArray{"1/16", "1/8", "1/4", "1/2", "1/1", "1/16T", "1/8T", "1/4T", "1/16D", "1/8D", "1/4D"}, 2));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FB_PITCH_SNAP, 1}, "Pitch Snap", false));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::FB_SCALE, 1}, "Scale",
        juce::StringArray{"Chromatic", "Major", "Minor", "Pentatonic Maj", "Pentatonic Min", "Blues", "Dorian", "Mixolydian", "Harmonic Min", "Whole Tone"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::FB_KEY, 1}, "Key",
        juce::StringArray{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::FB_ENABLED, 1}, "Feedback Enabled", false));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_ENABLED, 1}, "Multiband Enabled", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MB_LOW_FREQ, 1}, "MB Low Freq",
        juce::NormalisableRange<float>(20.0f, 2000.0f, 1.0f, 0.4f), 200.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MB_HIGH_FREQ, 1}, "MB High Freq",
        juce::NormalisableRange<float>(500.0f, 20000.0f, 1.0f, 0.4f), 5000.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_LOW_DIST, 1}, "MB Low Distortion", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_MID_DIST, 1}, "MB Mid Distortion", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_HIGH_DIST, 1}, "MB High Distortion", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_LOW_FILTER, 1}, "MB Low Filter", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_MID_FILTER, 1}, "MB Mid Filter", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_HIGH_FILTER, 1}, "MB High Filter", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_LOW_FB, 1}, "MB Low Feedback", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_MID_FB, 1}, "MB Mid Feedback", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MB_HIGH_FB, 1}, "MB High Feedback", true));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MOD_ENV_ATTACK, 1}, "Envelope Attack",
        juce::NormalisableRange<float>(0.1f, 500.0f, 0.1f, 0.4f), 10.0f, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MOD_ENV_RELEASE, 1}, "Envelope Release",
        juce::NormalisableRange<float>(1.0f, 2000.0f, 0.1f, 0.4f), 100.0f, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MOD_LFO_RATE, 1}, "LFO Rate",
        juce::NormalisableRange<float>(0.01f, 50.0f, 0.01f, 0.3f), 1.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::MOD_LFO_SHAPE, 1}, "LFO Shape",
        juce::StringArray{"Sine", "Triangle", "Saw Up", "Saw Down", "Square", "Random S&H"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MOD_LFO_MORPH, 1}, "LFO Morph",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ParamIDs::MOD_LFO_SYNC, 1}, "LFO Sync", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MACRO_1, 1}, "Macro 1",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ParamIDs::MACRO_2, 1}, "Macro 2",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID{ParamIDs::OVERSAMPLING, 1}, "Oversampling",
        juce::StringArray{"Off", "2x", "4x", "8x"}, 1));

    return {params.begin(), params.end()};
}

}
