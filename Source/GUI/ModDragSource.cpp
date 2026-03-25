#include "ModDragSource.h"
#include "GVNRLookAndFeel.h"

namespace gvnr {

ModDragSource::ModDragSource(ModSource source, const juce::String& label)
    : modSource(source), sourceLabel(label)
{
}

void ModDragSource::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(1.0f);

    g.setColour(Colors::highlight.withAlpha(0.6f));
    g.fillRoundedRectangle(bounds, 4.0f);
    g.setColour(Colors::highlight);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

    g.setColour(Colors::textPrimary);
    g.setFont(juce::Font(juce::FontOptions(9.0f)));
    g.drawText(sourceLabel, getLocalBounds(), juce::Justification::centred);
}

void ModDragSource::mouseDrag(const juce::MouseEvent& e)
{
    if (e.getDistanceFromDragStart() < 4)
        return;

    auto* container = juce::DragAndDropContainer::findParentDragContainerFor(this);
    if (container == nullptr) return;

    juce::String desc = "MOD_SOURCE:" + juce::String(static_cast<int>(modSource));
    container->startDragging(desc, this);
}

}
