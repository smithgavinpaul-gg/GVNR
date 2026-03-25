#include "PlayView.h"
#include "GVNRLookAndFeel.h"
#include "BinaryData.h"
#include "../PluginProcessor.h"

namespace gvnr {

static juce::Image stripBlackBackground(const juce::Image& src)
{
    auto result = src.createCopy();
    juce::Image::BitmapData pixels(result, juce::Image::BitmapData::readWrite);

    for (int y = 0; y < pixels.height; ++y) {
        for (int x = 0; x < pixels.width; ++x) {
            auto c = pixels.getPixelColour(x, y);
            float brightness = c.getBrightness();
            if (brightness < 0.08f)
                pixels.setPixelColour(x, y, juce::Colours::transparentBlack);
            else if (brightness < 0.2f) {
                float alpha = (brightness - 0.08f) / 0.12f;
                pixels.setPixelColour(x, y, c.withAlpha(alpha * c.getFloatAlpha()));
            }
        }
    }
    return result;
}

PlayView::PlayView(TheGVNRProcessor& processor)
    : driveKnob(processor.getAPVTS(), "distDrive", "DRIVE"),
      dryWetKnob(processor.getAPVTS(), "dryWet", "DRY/WET"),
      macro1Knob(processor.getAPVTS(), "macro1", "MACRO 1"),
      macro2Knob(processor.getAPVTS(), "macro2", "MACRO 2"),
      inputKnob(processor.getAPVTS(), "inputGain", "INPUT"),
      outputKnob(processor.getAPVTS(), "outputGain", "OUTPUT")
{
    addAndMakeVisible(driveKnob);
    addAndMakeVisible(dryWetKnob);
    addAndMakeVisible(macro1Knob);
    addAndMakeVisible(macro2Knob);
    addAndMakeVisible(inputKnob);
    addAndMakeVisible(outputKnob);
    addAndMakeVisible(waveformDisplay);

    titleLabel.setText("GVNRs Grunger", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(28.0f).withStyle("Bold")));
    titleLabel.setColour(juce::Label::textColourId, Colors::accent);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    presetLabel.setText("INIT", juce::dontSendNotification);
    presetLabel.setFont(juce::Font(juce::FontOptions(14.0f)));
    presetLabel.setColour(juce::Label::textColourId, Colors::textPrimary);
    presetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(presetLabel);

    waveformDisplay.setSource(processor.getWaveformData(),
                               &processor.getWaveformWritePosAtomic(),
                               TheGVNRProcessor::kWaveformBufferSize);

    auto rawImg = juce::ImageCache::getFromMemory(BinaryData::gvnr_background_png,
                                                   BinaryData::gvnr_background_pngSize);
    backgroundImg = stripBlackBackground(rawImg);
}

void PlayView::setModulationMatrix(ModulationMatrix* matrix)
{
    driveKnob.setModulationMatrix(matrix);
    dryWetKnob.setModulationMatrix(matrix);
    macro1Knob.setModulationMatrix(matrix);
    macro2Knob.setModulationMatrix(matrix);
    inputKnob.setModulationMatrix(matrix);
    outputKnob.setModulationMatrix(matrix);
}

void PlayView::paint(juce::Graphics& g)
{
    g.fillAll(Colors::background);

    auto waveBounds = waveformDisplay.getBounds().toFloat().expanded(60.0f, 30.0f);
    juce::ColourGradient glow(Colors::accent.withAlpha(0.06f),
                               waveBounds.getCentreX(), waveBounds.getCentreY(),
                               juce::Colours::transparentBlack,
                               waveBounds.getX(), waveBounds.getCentreY(), true);
    g.setGradientFill(glow);
    g.fillEllipse(waveBounds);

    if (backgroundImg.isValid() && !logoArea.isEmpty()) {
        float imgAspect = static_cast<float>(backgroundImg.getWidth()) / static_cast<float>(backgroundImg.getHeight());
        float drawW = static_cast<float>(logoArea.getWidth()) * 0.7f;
        float drawH = drawW / imgAspect;

        float drawX = static_cast<float>(logoArea.getCentreX()) - drawW * 0.5f;
        float drawY = static_cast<float>(logoArea.getBottom()) - drawH;

        g.drawImage(backgroundImg,
                     static_cast<int>(drawX), static_cast<int>(drawY),
                     static_cast<int>(drawW), static_cast<int>(drawH),
                     0, 0, backgroundImg.getWidth(), backgroundImg.getHeight());
    }
}

void PlayView::resized()
{
    auto bounds = getLocalBounds().reduced(20);

    titleLabel.setBounds(bounds.removeFromTop(40));
    presetLabel.setBounds(bounds.removeFromTop(24));
    bounds.removeFromTop(12);

    waveformDisplay.setBounds(bounds.removeFromTop(140));
    bounds.removeFromTop(20);

    auto knobRow = bounds.removeFromTop(120);
    int knobW = knobRow.getWidth() / 6;
    inputKnob.setBounds(knobRow.removeFromLeft(knobW));
    driveKnob.setBounds(knobRow.removeFromLeft(knobW));
    macro1Knob.setBounds(knobRow.removeFromLeft(knobW));
    macro2Knob.setBounds(knobRow.removeFromLeft(knobW));
    dryWetKnob.setBounds(knobRow.removeFromLeft(knobW));
    outputKnob.setBounds(knobRow);

    auto fullBounds = getLocalBounds();
    logoArea = fullBounds.removeFromBottom(fullBounds.getHeight() / 2);
}

}
