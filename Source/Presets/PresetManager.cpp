#include "PresetManager.h"
#include "FactoryPresets.h"
#include <random>

namespace gvnr {

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& state)
    : apvts(state)
{
    createPresetDirectory();
    installFactoryPresets();
    installPresetMetadata();
    loadFavorites();
}

juce::File PresetManager::getPresetDirectory() const
{
    auto dir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                   .getChildFile("GVNR Audio")
                   .getChildFile("The GVNR")
                   .getChildFile("Presets");
    return dir;
}

juce::File PresetManager::getMetadataFile() const
{
    return getPresetDirectory().getChildFile("presets-meta.xml");
}

juce::File PresetManager::getFavoritesFile() const
{
    return getPresetDirectory().getChildFile("favorites.xml");
}

void PresetManager::createPresetDirectory() const
{
    auto dir = getPresetDirectory();
    if (!dir.exists())
        dir.createDirectory();
}

void PresetManager::savePreset(const juce::String& name)
{
    auto state = apvts.copyState();
    auto xml = state.createXml();
    if (xml != nullptr) {
        auto file = getPresetDirectory().getChildFile(name + ".gvnr");
        xml->writeTo(file);
    }
}

void PresetManager::loadPreset(const juce::String& name)
{
    auto file = getPresetDirectory().getChildFile(name + ".gvnr");
    if (file.existsAsFile()) {
        auto xml = juce::XmlDocument::parse(file);
        if (xml != nullptr) {
            auto newState = juce::ValueTree::fromXml(*xml);
            if (newState.isValid())
                apvts.replaceState(newState);
        }
    }
}

void PresetManager::deletePreset(const juce::String& name)
{
    auto file = getPresetDirectory().getChildFile(name + ".gvnr");
    if (file.existsAsFile())
        file.deleteFile();
    presetMetadata.erase(name);
}

juce::StringArray PresetManager::getPresetNames() const
{
    juce::StringArray names;
    auto dir = getPresetDirectory();
    for (const auto& entry : juce::RangedDirectoryIterator(dir, false, "*.gvnr")) {
        names.add(entry.getFile().getFileNameWithoutExtension());
    }
    names.sort(false);
    return names;
}

juce::StringArray PresetManager::getPresetNamesForCategory(const juce::String& category) const
{
    if (category.isEmpty() || category == "All")
        return getPresetNames();

    if (category == "Favorites")
        return getFavorites();

    juce::StringArray result;
    for (const auto& [name, meta] : presetMetadata) {
        if (meta.category.equalsIgnoreCase(category)) {
            auto file = getPresetDirectory().getChildFile(name + ".gvnr");
            if (file.existsAsFile())
                result.add(name);
        }
    }
    result.sort(false);
    return result;
}

juce::StringArray PresetManager::getCategories() const
{
    juce::StringArray cats;
    cats.add("All");
    cats.add("Favorites");

    juce::StringArray uniqueCats;
    for (const auto& [name, meta] : presetMetadata) {
        if (meta.category.isNotEmpty() && !uniqueCats.contains(meta.category))
            uniqueCats.add(meta.category);
    }
    uniqueCats.sort(false);
    cats.addArray(uniqueCats);
    return cats;
}

juce::String PresetManager::getCategoryForPreset(const juce::String& name) const
{
    auto it = presetMetadata.find(name);
    if (it != presetMetadata.end())
        return it->second.category;
    return {};
}

void PresetManager::toggleFavorite(const juce::String& name)
{
    if (favoritePresets.contains(name))
        favoritePresets.removeString(name);
    else
        favoritePresets.add(name);
    saveFavorites();
}

bool PresetManager::isFavorite(const juce::String& name) const
{
    return favoritePresets.contains(name);
}

juce::StringArray PresetManager::getFavorites() const
{
    juce::StringArray result;
    for (const auto& name : favoritePresets) {
        auto file = getPresetDirectory().getChildFile(name + ".gvnr");
        if (file.existsAsFile())
            result.add(name);
    }
    result.sort(false);
    return result;
}

juce::StringArray PresetManager::getTagsForPreset(const juce::String& name) const
{
    auto it = presetMetadata.find(name);
    if (it != presetMetadata.end())
        return it->second.tags;
    return {};
}

void PresetManager::setTagsForPreset(const juce::String& name, const juce::StringArray& tags)
{
    presetMetadata[name].tags = tags;

    auto metaFile = getMetadataFile();
    auto xml = std::make_unique<juce::XmlElement>("PresetMetadata");
    for (const auto& [pName, meta] : presetMetadata) {
        auto* preset = xml->createNewChildElement("Preset");
        preset->setAttribute("name", pName);
        preset->setAttribute("category", meta.category);
        preset->setAttribute("tags", meta.tags.joinIntoString(","));
    }
    xml->writeTo(metaFile);
}

juce::StringArray PresetManager::searchByTag(const juce::String& tag) const
{
    juce::StringArray result;
    for (const auto& [name, meta] : presetMetadata) {
        for (const auto& t : meta.tags) {
            if (t.containsIgnoreCase(tag)) {
                auto file = getPresetDirectory().getChildFile(name + ".gvnr");
                if (file.existsAsFile())
                    result.add(name);
                break;
            }
        }
    }
    result.sort(false);
    return result;
}

void PresetManager::randomizeParameters()
{
    std::mt19937 gen{std::random_device{}()};

    for (auto* param : apvts.processor.getParameters()) {
        if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*>(param)) {
            auto id = rangedParam->getParameterID();

            if (id == "bypass" || id == "inputGain" || id == "outputGain")
                continue;

            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            float randomVal = dist(gen);

            if (id.contains("Enabled") || id.contains("PingPong") || id.contains("Sync")
                || id.contains("PitchSnap") || id.contains("mb"))
            {
                randomVal = (randomVal > 0.5f) ? 1.0f : 0.0f;
            }

            rangedParam->setValueNotifyingHost(randomVal);
        }
    }
}

void PresetManager::installFactoryPresets()
{
    auto dir = getPresetDirectory();
    auto defaultState = apvts.copyState();

    for (const auto& preset : FactoryPresets::getPresets()) {
        auto file = dir.getChildFile(juce::String(preset.name) + ".gvnr");
        if (file.existsAsFile())
            continue;

        auto tree = defaultState.createCopy();
        for (const auto& [paramId, value] : preset.overrides) {
            for (int i = 0; i < tree.getNumChildren(); ++i) {
                auto child = tree.getChild(i);
                if (child.getProperty("id").toString() == paramId) {
                    child.setProperty("value", value, nullptr);
                    break;
                }
            }
        }

        if (auto xml = tree.createXml())
            xml->writeTo(file);
    }
}

void PresetManager::installPresetMetadata()
{
    auto metaFile = getMetadataFile();
    if (metaFile.existsAsFile()) {
        auto xml = juce::XmlDocument::parse(metaFile);
        if (xml != nullptr) {
            for (auto* child : xml->getChildIterator()) {
                auto name = child->getStringAttribute("name");
                PresetMeta meta;
                meta.category = child->getStringAttribute("category");
                auto tagStr = child->getStringAttribute("tags");
                if (tagStr.isNotEmpty())
                    meta.tags.addTokens(tagStr, ",", "");
                presetMetadata[name] = meta;
            }
            return;
        }
    }

    for (const auto& preset : FactoryPresets::getPresets()) {
        PresetMeta meta;
        meta.category = juce::String(preset.category);
        meta.tags.add(juce::String(preset.category).toLowerCase());
        presetMetadata[juce::String(preset.name)] = meta;
    }

    auto xml = std::make_unique<juce::XmlElement>("PresetMetadata");
    for (const auto& [name, meta] : presetMetadata) {
        auto* presetEl = xml->createNewChildElement("Preset");
        presetEl->setAttribute("name", name);
        presetEl->setAttribute("category", meta.category);
        presetEl->setAttribute("tags", meta.tags.joinIntoString(","));
    }
    xml->writeTo(metaFile);
}

void PresetManager::loadFavorites()
{
    auto file = getFavoritesFile();
    if (!file.existsAsFile()) return;

    auto xml = juce::XmlDocument::parse(file);
    if (xml == nullptr) return;

    for (auto* child : xml->getChildIterator()) {
        auto name = child->getStringAttribute("name");
        if (name.isNotEmpty())
            favoritePresets.add(name);
    }
}

void PresetManager::saveFavorites() const
{
    auto xml = std::make_unique<juce::XmlElement>("Favorites");
    for (const auto& name : favoritePresets) {
        auto* child = xml->createNewChildElement("Preset");
        child->setAttribute("name", name);
    }
    xml->writeTo(getFavoritesFile());
}

}
