#include "PresetBrowser.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

PresetBrowser::PresetBrowser(PresetManager& pm) : presetManager(pm)
{
    prevButton.setButtonText("<");
    nextButton.setButtonText(">");
    saveButton.setButtonText("Save");
    randomButton.setButtonText("Rnd");
    favoriteButton.setButtonText("*");
    favoriteButton.setClickingTogglesState(true);

    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(randomButton);
    addAndMakeVisible(favoriteButton);

    presetNameLabel.setText("INIT", juce::dontSendNotification);
    presetNameLabel.setFont(juce::Font(juce::FontOptions(14.0f)));
    presetNameLabel.setColour(juce::Label::textColourId, Colors::textPrimary);
    presetNameLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(presetNameLabel);

    presetList.setTextWhenNothingSelected("Select Preset...");
    addAndMakeVisible(presetList);

    categoryFilter.setTextWhenNothingSelected("All");
    addAndMakeVisible(categoryFilter);

    searchField.setTextToShowWhenEmpty("Search tags...", Colors::textSecondary);
    searchField.setFont(juce::Font(juce::FontOptions(12.0f)));
    searchField.setColour(juce::TextEditor::backgroundColourId, Colors::surface);
    searchField.setColour(juce::TextEditor::textColourId, Colors::textPrimary);
    searchField.setColour(juce::TextEditor::outlineColourId, Colors::divider);
    addAndMakeVisible(searchField);

    refreshList();

    prevButton.onClick = [this]() {
        if (!filteredPresets.isEmpty()) {
            currentIndex = (currentIndex - 1 + filteredPresets.size()) % filteredPresets.size();
            presetManager.loadPreset(filteredPresets[currentIndex]);
            presetNameLabel.setText(filteredPresets[currentIndex], juce::dontSendNotification);
            favoriteButton.setToggleState(presetManager.isFavorite(filteredPresets[currentIndex]),
                                          juce::dontSendNotification);
        }
    };

    nextButton.onClick = [this]() {
        if (!filteredPresets.isEmpty()) {
            currentIndex = (currentIndex + 1) % filteredPresets.size();
            presetManager.loadPreset(filteredPresets[currentIndex]);
            presetNameLabel.setText(filteredPresets[currentIndex], juce::dontSendNotification);
            favoriteButton.setToggleState(presetManager.isFavorite(filteredPresets[currentIndex]),
                                          juce::dontSendNotification);
        }
    };

    saveButton.onClick = [this]() {
        auto name = presetNameLabel.getText();
        if (name.isNotEmpty())
            presetManager.savePreset(name);
    };

    randomButton.onClick = [this]() {
        presetManager.randomizeParameters();
        presetNameLabel.setText("Random", juce::dontSendNotification);
    };

    favoriteButton.onClick = [this]() {
        auto name = presetNameLabel.getText();
        if (name.isNotEmpty() && name != "INIT" && name != "Random") {
            presetManager.toggleFavorite(name);
            favoriteButton.setToggleState(presetManager.isFavorite(name), juce::dontSendNotification);
            applyFilter();
        }
    };

    presetList.onChange = [this]() {
        auto name = presetList.getText();
        if (name.isNotEmpty()) {
            presetManager.loadPreset(name);
            presetNameLabel.setText(name, juce::dontSendNotification);
            favoriteButton.setToggleState(presetManager.isFavorite(name), juce::dontSendNotification);
        }
    };

    categoryFilter.onChange = [this]() {
        applyFilter();
    };

    searchField.onReturnKey = [this]() {
        applyFilter();
    };

    searchField.onFocusLost = [this]() {
        applyFilter();
    };
}

void PresetBrowser::applyFilter()
{
    auto category = categoryFilter.getText();
    auto searchTerm = searchField.getText().trim();

    if (searchTerm.isNotEmpty()) {
        filteredPresets = presetManager.searchByTag(searchTerm);
    } else {
        filteredPresets = presetManager.getPresetNamesForCategory(category);
    }

    presetList.clear(juce::dontSendNotification);
    for (int i = 0; i < filteredPresets.size(); ++i)
        presetList.addItem(filteredPresets[i], i + 1);

    currentIndex = 0;
}

void PresetBrowser::refreshList()
{
    categoryFilter.clear(juce::dontSendNotification);
    auto cats = presetManager.getCategories();
    for (int i = 0; i < cats.size(); ++i)
        categoryFilter.addItem(cats[i], i + 1);
    categoryFilter.setSelectedId(1, juce::dontSendNotification);

    filteredPresets = presetManager.getPresetNames();
    presetList.clear(juce::dontSendNotification);
    for (int i = 0; i < filteredPresets.size(); ++i)
        presetList.addItem(filteredPresets[i], i + 1);
}

void PresetBrowser::paint(juce::Graphics& g)
{
    GlassPanel::paint(g, getLocalBounds().toFloat(), 8.0f);
}

void PresetBrowser::resized()
{
    auto bounds = getLocalBounds().reduced(8);

    auto topRow = bounds.removeFromTop(24);
    prevButton.setBounds(topRow.removeFromLeft(26));
    nextButton.setBounds(topRow.removeFromRight(26));
    favoriteButton.setBounds(topRow.removeFromRight(26));
    topRow.removeFromRight(4);
    presetNameLabel.setBounds(topRow);

    bounds.removeFromTop(4);
    auto midRow = bounds.removeFromTop(24);
    categoryFilter.setBounds(midRow.removeFromLeft(midRow.getWidth() / 2 - 2));
    midRow.removeFromLeft(4);
    searchField.setBounds(midRow);

    bounds.removeFromTop(4);
    auto bottomRow = bounds.removeFromTop(24);
    presetList.setBounds(bottomRow.removeFromLeft(bottomRow.getWidth() - 100));
    bottomRow.removeFromLeft(4);
    saveButton.setBounds(bottomRow.removeFromLeft(46));
    bottomRow.removeFromLeft(4);
    randomButton.setBounds(bottomRow);
}

}
