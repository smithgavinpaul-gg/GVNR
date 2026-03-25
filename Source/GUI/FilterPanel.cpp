#include "FilterPanel.h"
#include "GVNRLookAndFeel.h"
#include "BinaryData.h"

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

static juce::StringArray getFilterNames()
{
    return {
        "LP 12dB", "LP 24dB", "HP 12dB", "HP 24dB",
        "Band Pass", "Notch", "All Pass", "Peak",
        "Low Shelf", "High Shelf", "Comb+", "Comb-",
        "Phaser 1", "Phaser 2", "Phaser 4", "Phaser 6",
        "Vowel A", "Vowel E", "Vowel I", "Vowel O",
        "Formant", "Resonator", "LP>HP Morph", "LP>BP Morph"
    };
}

FilterPanel::FilterPanel(juce::AudioProcessorValueTreeState& apvts, bool isPre)
    : cutoffKnob(apvts, isPre ? "filterPreCutoff" : "filterPostCutoff", "CUTOFF"),
      resoKnob(apvts, isPre ? "filterPreReso" : "filterPostReso", "RESO"),
      morphKnob(apvts, isPre ? "filterPreMorph" : "filterPostMorph", "MORPH"),
      isPreFilter(isPre)
{
    addAndMakeVisible(cutoffKnob);
    addAndMakeVisible(resoKnob);
    addAndMakeVisible(morphKnob);

    typeBox.addItemList(getFilterNames(), 1);
    addAndMakeVisible(typeBox);

    enableButton.setButtonText(isPre ? "Pre-Filter" : "Post-Filter");
    addAndMakeVisible(enableButton);

    sectionLabel.setText(isPre ? "PRE-FILTER" : "POST-FILTER", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(juce::FontOptions(11.0f)));
    sectionLabel.setColour(juce::Label::textColourId, Colors::textSecondary);
    sectionLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(sectionLabel);

    typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, isPre ? "filterPreType" : "filterPostType", typeBox);
    enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, isPre ? "filterPreEnabled" : "filterPostEnabled", enableButton);

    if (!isPre) {
        auto rawImg = juce::ImageCache::getFromMemory(BinaryData::gvnr_background_png,
                                                       BinaryData::gvnr_background_pngSize);
        backgroundImg = stripBlackBackground(rawImg);
    }
}

void FilterPanel::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat());

    if (!isPreFilter && backgroundImg.isValid() && !logoArea.isEmpty()) {
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

void FilterPanel::resized()
{
    auto bounds = getLocalBounds().reduced(12);

    auto headerRow = bounds.removeFromTop(24);
    sectionLabel.setBounds(headerRow.removeFromLeft(headerRow.getWidth() / 2));
    enableButton.setBounds(headerRow);

    bounds.removeFromTop(6);
    typeBox.setBounds(bounds.removeFromTop(28));

    bounds.removeFromTop(8);
    auto knobRow = bounds.removeFromTop(90);
    int knobW = knobRow.getWidth() / 3;
    cutoffKnob.setBounds(knobRow.removeFromLeft(knobW));
    resoKnob.setBounds(knobRow.removeFromLeft(knobW));
    morphKnob.setBounds(knobRow);

    auto panelBounds = getLocalBounds().reduced(12);
    logoArea = panelBounds.removeFromBottom(panelBounds.getHeight() / 2);
}

}
