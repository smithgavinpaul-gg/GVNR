#include "DistortionAlgorithms.h"
#include <algorithm>
#include <juce_core/juce_core.h>

namespace gvnr {

thread_local std::mt19937 DistortionAlgorithms::rng{std::random_device{}()};
thread_local int DistortionAlgorithms::decimateCounter = 0;
thread_local float DistortionAlgorithms::decimateHeld = 0.0f;
thread_local float DistortionAlgorithms::driftPhase = 0.0f;

float DistortionAlgorithms::process(DistortionType type, float sample, float drive, float mix)
{
    float d = std::clamp(drive, 0.0f, 1.0f);
    float processed = sample;

    switch (type) {
        case DistortionType::SoftClip:        processed = softClip(sample, d); break;
        case DistortionType::HardClip:        processed = hardClip(sample, d); break;
        case DistortionType::TubeSaturation:  processed = tubeSaturation(sample, d); break;
        case DistortionType::TapeSaturation:  processed = tapeSaturation(sample, d); break;
        case DistortionType::Transistor:      processed = transistor(sample, d); break;
        case DistortionType::AsymmetricClip:  processed = asymmetricClip(sample, d); break;
        case DistortionType::SineFold:        processed = sineFold(sample, d); break;
        case DistortionType::TriangleFold:    processed = triangleFold(sample, d); break;
        case DistortionType::Chebyshev:       processed = chebyshev(sample, d); break;
        case DistortionType::Parabolic:       processed = parabolic(sample, d); break;
        case DistortionType::ExponentialFold: processed = exponentialFold(sample, d); break;
        case DistortionType::MultiFold:       processed = multiFold(sample, d); break;
        case DistortionType::NoiseGateCrush:  processed = noiseGateCrush(sample, d); break;
        case DistortionType::NoiseBlend:      processed = noiseBlend(sample, d); break;
        case DistortionType::NoiseRingMod:    processed = noiseRingMod(sample, d); break;
        case DistortionType::NoiseAM:         processed = noiseAM(sample, d); break;
        case DistortionType::GranularNoise:   processed = granularNoise(sample, d); break;
        case DistortionType::Static:          processed = staticNoise(sample, d); break;
        case DistortionType::Bit4:            processed = bit4(sample, d); break;
        case DistortionType::Bit8:            processed = bit8(sample, d); break;
        case DistortionType::Bit12:           processed = bit12(sample, d); break;
        case DistortionType::VariableBit:     processed = variableBit(sample, d); break;
        case DistortionType::Stepped:         processed = stepped(sample, d); break;
        case DistortionType::Quantize:        processed = quantize(sample, d); break;
        case DistortionType::Decimate2x:      processed = decimate2x(sample, d); break;
        case DistortionType::Decimate4x:      processed = decimate4x(sample, d); break;
        case DistortionType::Decimate8x:      processed = decimate8x(sample, d); break;
        case DistortionType::VariableRate:    processed = variableRate(sample, d); break;
        case DistortionType::Jitter:          processed = jitter(sample, d); break;
        case DistortionType::Drift:           processed = drift(sample, d); break;
        default: break;
    }

    return sample + mix * (processed - sample);
}

float DistortionAlgorithms::softClip(float x, float drive) {
    float gain = 1.0f + drive * 20.0f;
    x *= gain;
    return std::tanh(x);
}

float DistortionAlgorithms::hardClip(float x, float drive) {
    float gain = 1.0f + drive * 20.0f;
    float threshold = 1.0f - drive * 0.8f;
    x *= gain;
    return std::clamp(x, -threshold, threshold);
}

float DistortionAlgorithms::tubeSaturation(float x, float drive) {
    float gain = 1.0f + drive * 15.0f;
    x *= gain;
    if (x >= 0.0f)
        return 1.0f - std::exp(-x);
    else
        return -(1.0f - std::exp(x));
}

float DistortionAlgorithms::tapeSaturation(float x, float drive) {
    float gain = 1.0f + drive * 10.0f;
    x *= gain;
    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

float DistortionAlgorithms::transistor(float x, float drive) {
    float gain = 1.0f + drive * 18.0f;
    x *= gain;
    return x / (1.0f + std::abs(x));
}

float DistortionAlgorithms::asymmetricClip(float x, float drive) {
    float gain = 1.0f + drive * 15.0f;
    x *= gain;
    if (x > 0.0f)
        return std::tanh(x);
    else
        return std::tanh(x * 0.5f) * 0.8f;
}

float DistortionAlgorithms::sineFold(float x, float drive) {
    float gain = 1.0f + drive * 8.0f;
    return std::sin(x * gain * juce::MathConstants<float>::pi);
}

float DistortionAlgorithms::triangleFold(float x, float drive) {
    float gain = 1.0f + drive * 8.0f;
    x *= gain;
    x = std::fmod(x + 1.0f, 4.0f);
    if (x < 0.0f) x += 4.0f;
    return (x < 2.0f) ? (x - 1.0f) : (3.0f - x);
}

float DistortionAlgorithms::chebyshev(float x, float drive) {
    float gain = 1.0f + drive * 5.0f;
    x = std::clamp(x * gain, -1.0f, 1.0f);
    float x2 = x * x;
    float t3 = x * (4.0f * x2 - 3.0f);
    float t5 = x * (16.0f * x2 * x2 - 20.0f * x2 + 5.0f);
    float blend = drive;
    return x * (1.0f - blend) + (t3 * 0.5f + t5 * 0.5f) * blend;
}

float DistortionAlgorithms::parabolic(float x, float drive) {
    float gain = 1.0f + drive * 10.0f;
    x *= gain;
    if (x > 0.0f)
        return 1.0f - (1.0f - x) * (1.0f - x);
    else
        return -1.0f + (1.0f + x) * (1.0f + x);
}

float DistortionAlgorithms::exponentialFold(float x, float drive) {
    float gain = 1.0f + drive * 6.0f;
    x *= gain;
    return std::sin(std::exp(std::abs(x)) - 1.0f) * (x >= 0.0f ? 1.0f : -1.0f);
}

float DistortionAlgorithms::multiFold(float x, float drive) {
    float gain = 1.0f + drive * 10.0f;
    x *= gain;
    int folds = 1 + static_cast<int>(drive * 4.0f);
    for (int i = 0; i < folds; ++i)
        x = std::sin(x * juce::MathConstants<float>::pi * 0.5f);
    return x;
}

float DistortionAlgorithms::noiseGateCrush(float x, float drive) {
    float threshold = 0.01f + (1.0f - drive) * 0.3f;
    if (std::abs(x) < threshold) return 0.0f;
    std::uniform_real_distribution<float> dist(-0.05f * drive, 0.05f * drive);
    return x + dist(rng);
}

float DistortionAlgorithms::noiseBlend(float x, float drive) {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return x * (1.0f - drive) + dist(rng) * drive * std::abs(x);
}

float DistortionAlgorithms::noiseRingMod(float x, float drive) {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    float noise = dist(rng);
    return x * (1.0f - drive * 0.8f) + x * noise * drive * 0.8f;
}

float DistortionAlgorithms::noiseAM(float x, float drive) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float modulator = 1.0f - drive * dist(rng);
    return x * modulator;
}

float DistortionAlgorithms::granularNoise(float x, float drive) {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    int grainSize = static_cast<int>(10.0f + (1.0f - drive) * 100.0f);
    if (decimateCounter % grainSize == 0) {
        decimateHeld = dist(rng) * drive;
    }
    ++decimateCounter;
    return x + decimateHeld * std::abs(x);
}

float DistortionAlgorithms::staticNoise(float x, float drive) {
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    return x + dist(rng) * drive * 0.5f;
}

static float bitReduce(float x, int bits) {
    float levels = std::pow(2.0f, static_cast<float>(bits));
    return std::round(x * levels) / levels;
}

float DistortionAlgorithms::bit4(float x, float drive) {
    float gain = 1.0f + drive * 5.0f;
    return bitReduce(x * gain, 4) / gain;
}

float DistortionAlgorithms::bit8(float x, float drive) {
    float gain = 1.0f + drive * 5.0f;
    return bitReduce(x * gain, 8) / gain;
}

float DistortionAlgorithms::bit12(float x, float drive) {
    float gain = 1.0f + drive * 5.0f;
    return bitReduce(x * gain, 12) / gain;
}

float DistortionAlgorithms::variableBit(float x, float drive) {
    int bits = static_cast<int>(16.0f - drive * 14.0f);
    bits = std::max(bits, 2);
    return bitReduce(x, bits);
}

float DistortionAlgorithms::stepped(float x, float drive) {
    float steps = 2.0f + (1.0f - drive) * 30.0f;
    return std::round(x * steps) / steps;
}

float DistortionAlgorithms::quantize(float x, float drive) {
    float q = 0.001f + (1.0f - drive) * 0.1f;
    return std::round(x / q) * q;
}

float DistortionAlgorithms::decimate2x(float x, float drive) {
    if (decimateCounter % 2 == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld * (1.0f + drive * 0.5f);
}

float DistortionAlgorithms::decimate4x(float x, float drive) {
    if (decimateCounter % 4 == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld * (1.0f + drive * 0.5f);
}

float DistortionAlgorithms::decimate8x(float x, float drive) {
    if (decimateCounter % 8 == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld * (1.0f + drive * 0.5f);
}

float DistortionAlgorithms::variableRate(float x, float drive) {
    int factor = 1 + static_cast<int>(drive * 15.0f);
    if (decimateCounter % factor == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld;
}

float DistortionAlgorithms::jitter(float x, float drive) {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(drive * 10.0f) + 1);
    int skip = dist(rng);
    if (decimateCounter % (skip + 1) == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld;
}

float DistortionAlgorithms::drift(float x, float drive) {
    driftPhase += 0.001f + drive * 0.01f;
    if (driftPhase > 1.0f) driftPhase -= 1.0f;
    float mod = std::sin(driftPhase * juce::MathConstants<float>::twoPi);
    int factor = 1 + static_cast<int>((1.0f + mod) * drive * 5.0f);
    if (decimateCounter % std::max(factor, 1) == 0)
        decimateHeld = x;
    ++decimateCounter;
    return decimateHeld;
}

}
