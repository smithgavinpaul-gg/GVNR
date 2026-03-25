#pragma once
#include <cmath>
#include <random>

namespace gvnr {

enum class DistortionType {
    SoftClip, HardClip, TubeSaturation, TapeSaturation, Transistor, AsymmetricClip,
    SineFold, TriangleFold, Chebyshev, Parabolic, ExponentialFold, MultiFold,
    NoiseGateCrush, NoiseBlend, NoiseRingMod, NoiseAM, GranularNoise, Static,
    Bit4, Bit8, Bit12, VariableBit, Stepped, Quantize,
    Decimate2x, Decimate4x, Decimate8x, VariableRate, Jitter, Drift,
    NumTypes
};

class DistortionAlgorithms {
public:
    static float process(DistortionType type, float sample, float drive, float mix);

private:
    static float softClip(float x, float drive);
    static float hardClip(float x, float drive);
    static float tubeSaturation(float x, float drive);
    static float tapeSaturation(float x, float drive);
    static float transistor(float x, float drive);
    static float asymmetricClip(float x, float drive);
    static float sineFold(float x, float drive);
    static float triangleFold(float x, float drive);
    static float chebyshev(float x, float drive);
    static float parabolic(float x, float drive);
    static float exponentialFold(float x, float drive);
    static float multiFold(float x, float drive);
    static float noiseGateCrush(float x, float drive);
    static float noiseBlend(float x, float drive);
    static float noiseRingMod(float x, float drive);
    static float noiseAM(float x, float drive);
    static float granularNoise(float x, float drive);
    static float staticNoise(float x, float drive);
    static float bit4(float x, float drive);
    static float bit8(float x, float drive);
    static float bit12(float x, float drive);
    static float variableBit(float x, float drive);
    static float stepped(float x, float drive);
    static float quantize(float x, float drive);
    static float decimate2x(float x, float drive);
    static float decimate4x(float x, float drive);
    static float decimate8x(float x, float drive);
    static float variableRate(float x, float drive);
    static float jitter(float x, float drive);
    static float drift(float x, float drive);

    static thread_local std::mt19937 rng;
    static thread_local int decimateCounter;
    static thread_local float decimateHeld;
    static thread_local float driftPhase;
};

}
