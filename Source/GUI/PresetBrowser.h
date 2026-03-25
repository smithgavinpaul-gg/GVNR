#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../Presets/PresetManager.h"

namespace gvnr {

class PresetBrowser : public juce::Component {
public:
    PresetBrowser(PresetManager& presetManager);
    void resized() override;
    void paint(juce::Graphics& g) override;

    void refreshList();

private:
    void applyFilter();

    PresetManager& presetManager;

    juce::TextButton prevButton;
    juce::TextButton nextButton;
    juce::TextButton saveButton;
    juce::TextButton randomButton;
    juce::TextButton favoriteButton;

    juce::Label presetNameLabel;
    juce::ComboBox presetList;
    juce::ComboBox categoryFilter;
    juce::TextEditor searchField;

    juce::StringArray filteredPresets;
    int currentIndex = 0;
};

}
