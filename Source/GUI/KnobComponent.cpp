#include "KnobComponent.h"
#include "GVNRLookAndFeel.h"
#include "../Modulation/ModulationMatrix.h"

namespace gvnr {

KnobComponent::KnobComponent(juce::AudioProcessorValueTreeState& apvts,
                               const juce::String& paramId,
                               const juce::String& labelText)
    : parameterID(paramId)
{
    knob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
    knob.setColour(juce::Slider::textBoxTextColourId, Colors::textPrimary);
    knob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(knob);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, Colors::textSecondary);
    label.setFont(juce::Font(juce::FontOptions(10.0f)));
    addAndMakeVisible(label);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, paramId, knob);
}

void KnobComponent::setModulationMatrix(ModulationMatrix* matrix)
{
    modMatrix = matrix;
}

void KnobComponent::paint(juce::Graphics& g)
{
    if (dragHovering) {
        g.setColour(Colors::highlight.withAlpha(0.2f));
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 6.0f);
    }

    if (modMatrix != nullptr) {
        const auto& routes = modMatrix->getRoutes();
        bool hasRoute = false;
        float totalAmount = 0.0f;
        for (const auto& route : routes) {
            if (route.targetParamId == parameterID.toStdString() && !route.bypassed) {
                hasRoute = true;
                totalAmount += route.amount;
            }
        }
        if (hasRoute) {
            auto knobBounds = knob.getBounds().toFloat();
            float diameter = std::min(knobBounds.getWidth(), knobBounds.getHeight()) * 0.8f;
            float centreX = knobBounds.getCentreX();
            float centreY = knobBounds.getCentreY();
            float radius = diameter * 0.5f + 4.0f;

            float modNorm = std::clamp(totalAmount / 100.0f, -1.0f, 1.0f);
            float dotAngle = -juce::MathConstants<float>::halfPi + modNorm * juce::MathConstants<float>::pi;
            float dotX = centreX + radius * std::cos(dotAngle);
            float dotY = centreY + radius * std::sin(dotAngle);

            g.setColour(Colors::highlight);
            g.fillEllipse(dotX - 3.0f, dotY - 3.0f, 6.0f, 6.0f);
        }
    }
}

bool KnobComponent::isInterestedInDragSource(const SourceDetails& details)
{
    return details.description.toString().startsWith("MOD_SOURCE:");
}

void KnobComponent::itemDragEnter(const SourceDetails&)
{
    dragHovering = true;
    repaint();
}

void KnobComponent::itemDragExit(const SourceDetails&)
{
    dragHovering = false;
    repaint();
}

void KnobComponent::itemDropped(const SourceDetails& details)
{
    dragHovering = false;

    if (modMatrix == nullptr) return;

    auto desc = details.description.toString();
    if (!desc.startsWith("MOD_SOURCE:")) return;

    int sourceInt = desc.fromLastOccurrenceOf(":", false, false).getIntValue();
    auto source = static_cast<ModSource>(sourceInt);

    ModRoute route;
    route.source = source;
    route.targetParamId = parameterID.toStdString();
    route.amount = 50.0f;
    route.bypassed = false;

    modMatrix->addRoute(route);
    repaint();
}

void KnobComponent::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu() && modMatrix != nullptr) {
        const auto& routes = modMatrix->getRoutes();
        juce::PopupMenu menu;
        int itemId = 1;
        std::vector<int> routeIndices;

        for (int i = 0; i < static_cast<int>(routes.size()); ++i) {
            if (routes[static_cast<size_t>(i)].targetParamId == parameterID.toStdString()) {
                auto srcName = [](ModSource s) -> juce::String {
                    switch (s) {
                        case ModSource::EnvFollower: return "Envelope";
                        case ModSource::LFO:        return "LFO";
                        case ModSource::CurveSeq1:  return "Curve Seq 1";
                        case ModSource::CurveSeq2:  return "Curve Seq 2";
                        case ModSource::Macro1:     return "Macro 1";
                        case ModSource::Macro2:     return "Macro 2";
                        default:                    return "Unknown";
                    }
                };
                menu.addItem(itemId, "Remove " + srcName(routes[static_cast<size_t>(i)].source));
                routeIndices.push_back(i);
                ++itemId;
            }
        }

        if (menu.getNumItems() > 0) {
            menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),
                [this, routeIndices](int result) {
                    if (result > 0 && result <= static_cast<int>(routeIndices.size())) {
                        modMatrix->removeRoute(routeIndices[static_cast<size_t>(result - 1)]);
                        repaint();
                    }
                });
            return;
        }
    }

    Component::mouseDown(e);
}

void KnobComponent::resized()
{
    auto bounds = getLocalBounds();
    label.setBounds(bounds.removeFromTop(16));
    knob.setBounds(bounds);
}

}
