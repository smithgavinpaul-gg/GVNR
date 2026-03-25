#include "MainPanel.h"
#include "GVNRLookAndFeel.h"
#include "BinaryData.h"
#include "../PluginProcessor.h"

namespace gvnr {

MainPanel::MainPanel(TheGVNRProcessor& p, GVNRLookAndFeel& lnf)
    : processor(p),
      lookAndFeel(lnf),
      playView(p),
      advancedView(p),
      presetBrowser(p.getPresetManager())
{
    logo = juce::ImageCache::getFromMemory(BinaryData::gvnr_logo_png,
                                            BinaryData::gvnr_logo_pngSize);
    background = juce::ImageCache::getFromMemory(BinaryData::gvnr_background_png,
                                                  BinaryData::gvnr_background_pngSize);

    addAndMakeVisible(playView);
    addChildComponent(advancedView);
    addAndMakeVisible(presetBrowser);
    addAndMakeVisible(tooltip);

    playViewBtn.setButtonText("Play");
    advancedViewBtn.setButtonText("Advanced");
    playViewBtn.setToggleState(true, juce::dontSendNotification);

    themeToggleBtn.setButtonText("Light");
    themeToggleBtn.setClickingTogglesState(true);

    addAndMakeVisible(playViewBtn);
    addAndMakeVisible(advancedViewBtn);
    addAndMakeVisible(themeToggleBtn);

    playViewBtn.onClick = [this]() {
        showAdvanced = false;
        playView.setVisible(true);
        advancedView.setVisible(false);
        playViewBtn.setToggleState(true, juce::dontSendNotification);
        advancedViewBtn.setToggleState(false, juce::dontSendNotification);
    };

    advancedViewBtn.onClick = [this]() {
        showAdvanced = true;
        playView.setVisible(false);
        advancedView.setVisible(true);
        playViewBtn.setToggleState(false, juce::dontSendNotification);
        advancedViewBtn.setToggleState(true, juce::dontSendNotification);
    };

    themeToggleBtn.onClick = [this]() {
        bool goLight = themeToggleBtn.getToggleState();
        lookAndFeel.setDarkMode(!goLight);
        themeToggleBtn.setButtonText(goLight ? "Dark" : "Light");
        getTopLevelComponent()->repaint();
    };

    auto* modMatrix = &processor.getModMatrix();
    playView.setModulationMatrix(modMatrix);
    advancedView.setModulationMatrix(modMatrix);
}

void MainPanel::paint(juce::Graphics& g)
{
    g.fillAll(Colors::background);

    auto topBar = getLocalBounds().removeFromTop(44).toFloat();
    g.setColour(Colors::surface.withAlpha(0.4f));
    g.fillRect(topBar);

    g.setColour(Colors::glassBorder);
    g.drawLine(topBar.getX(), topBar.getBottom(), topBar.getRight(), topBar.getBottom(), 1.0f);
}

void MainPanel::resized()
{
    auto bounds = getLocalBounds();

    auto topBar = bounds.removeFromTop(44);
    topBar = topBar.reduced(10, 6);

    auto btnArea = topBar.removeFromLeft(200);
    playViewBtn.setBounds(btnArea.removeFromLeft(95));
    btnArea.removeFromLeft(10);
    advancedViewBtn.setBounds(btnArea);

    themeToggleBtn.setBounds(topBar.removeFromRight(60));
    topBar.removeFromRight(8);

    presetBrowser.setBounds(topBar);

    playView.setBounds(bounds);
    advancedView.setBounds(bounds);
    tooltip.setBounds(getLocalBounds());
}

}
