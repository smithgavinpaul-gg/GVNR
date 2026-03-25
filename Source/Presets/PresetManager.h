#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace gvnr {

class PresetManager {
public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);

    void savePreset(const juce::String& name);
    void loadPreset(const juce::String& name);
    void deletePreset(const juce::String& name);

    juce::StringArray getPresetNames() const;
    juce::StringArray getPresetNamesForCategory(const juce::String& category) const;
    juce::StringArray getCategories() const;
    juce::String getCategoryForPreset(const juce::String& name) const;

    void toggleFavorite(const juce::String& name);
    bool isFavorite(const juce::String& name) const;
    juce::StringArray getFavorites() const;

    juce::StringArray getTagsForPreset(const juce::String& name) const;
    void setTagsForPreset(const juce::String& name, const juce::StringArray& tags);
    juce::StringArray searchByTag(const juce::String& tag) const;

    void randomizeParameters();

    juce::File getPresetDirectory() const;

private:
    juce::AudioProcessorValueTreeState& apvts;

    juce::StringArray favoritePresets;

    void createPresetDirectory() const;
    void installFactoryPresets();
    void loadFavorites();
    void saveFavorites() const;
    void installPresetMetadata();

    juce::File getMetadataFile() const;
    juce::File getFavoritesFile() const;

    struct PresetMeta {
        juce::String category;
        juce::StringArray tags;
    };
    std::map<juce::String, PresetMeta> presetMetadata;
};

}
