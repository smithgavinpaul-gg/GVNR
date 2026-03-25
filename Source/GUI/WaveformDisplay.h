#pragma once

#include <array>
#include <atomic>

#include <juce_gui_basics/juce_gui_basics.h>

namespace gvnr {

class WaveformDisplay : public juce::Component, public juce::Timer {
public:
    WaveformDisplay();
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    void pushSample(float sample);
    void pushBuffer(const float* data, int numSamples);
    void setSource(const float* ringBuffer, const std::atomic<int>* writePos, int ringSize);

private:
    static constexpr int BUFFER_SIZE = 512;
    std::array<float, BUFFER_SIZE> displayBuffer{};
    int writeIndex = 0;
    std::atomic<bool> needsRepaint{false};

    const float* sourceRing = nullptr;
    const std::atomic<int>* sourceWritePos = nullptr;
    int sourceRingSize = 0;
};

}
