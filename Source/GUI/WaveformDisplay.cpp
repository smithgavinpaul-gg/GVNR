#include "WaveformDisplay.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

WaveformDisplay::WaveformDisplay()
{
    startTimerHz(30);
}

WaveformDisplay::~WaveformDisplay()
{
    stopTimer();
}

void WaveformDisplay::pushSample(float sample)
{
    displayBuffer[static_cast<size_t>(writeIndex)] = sample;
    writeIndex = (writeIndex + 1) % BUFFER_SIZE;
    needsRepaint.store(true);
}

void WaveformDisplay::pushBuffer(const float* data, int numSamples)
{
    for (int i = 0; i < numSamples; ++i) {
        displayBuffer[static_cast<size_t>(writeIndex)] = data[i];
        writeIndex = (writeIndex + 1) % BUFFER_SIZE;
    }
    needsRepaint.store(true);
}

void WaveformDisplay::setSource(const float* ringBuffer, const std::atomic<int>* writePos, int ringSize)
{
    sourceRing = ringBuffer;
    sourceWritePos = writePos;
    sourceRingSize = ringSize;
}

void WaveformDisplay::timerCallback()
{
    if (sourceRing != nullptr && sourceWritePos != nullptr) {
        int srcPos = sourceWritePos->load(std::memory_order_relaxed);
        int samplesToRead = std::min(BUFFER_SIZE, sourceRingSize);
        int startIdx = (srcPos - samplesToRead + sourceRingSize) % sourceRingSize;
        for (int i = 0; i < samplesToRead; ++i) {
            int ri = (startIdx + i) % sourceRingSize;
            displayBuffer[static_cast<size_t>(i)] = sourceRing[ri];
        }
        writeIndex = samplesToRead % BUFFER_SIZE;
        repaint();
    } else if (needsRepaint.exchange(false)) {
        repaint();
    }
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);

    GlassPanel::paint(g, bounds, 8.0f);

    g.setColour(Colors::divider.withAlpha(0.4f));
    float centreY = bounds.getCentreY();
    g.drawHorizontalLine(static_cast<int>(centreY), bounds.getX(), bounds.getRight());

    juce::Path waveform;
    float xStep = bounds.getWidth() / static_cast<float>(BUFFER_SIZE);

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        int readIdx = (writeIndex + i) % BUFFER_SIZE;
        float sample = displayBuffer[static_cast<size_t>(readIdx)];
        float xPos = bounds.getX() + static_cast<float>(i) * xStep;
        float yPos = centreY - sample * bounds.getHeight() * 0.45f;

        if (i == 0)
            waveform.startNewSubPath(xPos, yPos);
        else
            waveform.lineTo(xPos, yPos);
    }

    juce::ColourGradient gradient(juce::Colour(0xffFCAF45), bounds.getX(), centreY,
                                   juce::Colour(0xff405DE6), bounds.getRight(), centreY, false);
    gradient.addColour(0.2, juce::Colour(0xffF77737));
    gradient.addColour(0.4, juce::Colour(0xffFD1D1D));
    gradient.addColour(0.55, juce::Colour(0xffE1306C));
    gradient.addColour(0.7, juce::Colour(0xffC13584));
    gradient.addColour(0.85, juce::Colour(0xff833AB4));

    // Bloom glow pass
    g.setGradientFill(gradient);
    g.saveState();
    g.setOpacity(0.3f);
    g.strokePath(waveform, juce::PathStrokeType(5.0f, juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));
    g.restoreState();

    // Main waveform stroke
    g.setGradientFill(gradient);
    g.strokePath(waveform, juce::PathStrokeType(2.0f));

    juce::Path fillPath(waveform);
    fillPath.lineTo(bounds.getRight(), centreY);
    fillPath.lineTo(bounds.getX(), centreY);
    fillPath.closeSubPath();

    juce::ColourGradient fillGrad(juce::Colour(0xffFCAF45).withAlpha(0.2f), bounds.getX(), bounds.getY(),
                                   juce::Colour(0xff405DE6).withAlpha(0.02f), bounds.getX(), bounds.getBottom(), false);
    fillGrad.addColour(0.4, juce::Colour(0xffE1306C).withAlpha(0.1f));
    g.setGradientFill(fillGrad);
    g.fillPath(fillPath);
}

}
