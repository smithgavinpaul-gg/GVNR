#include "CurveEditor.h"
#include "GVNRLookAndFeel.h"
#include <algorithm>

namespace gvnr {

CurveEditor::CurveEditor(CurveSequencer& seq) : sequencer(seq)
{
    setRepaintsOnMouseActivity(true);
}

juce::Point<float> CurveEditor::pointToScreen(const CurvePoint& p) const
{
    auto bounds = getLocalBounds().toFloat().reduced(8.0f);
    float x = bounds.getX() + p.x * bounds.getWidth();
    float y = bounds.getBottom() - (p.y + 1.0f) * 0.5f * bounds.getHeight();
    return {x, y};
}

CurvePoint CurveEditor::screenToPoint(juce::Point<float> screen) const
{
    auto bounds = getLocalBounds().toFloat().reduced(8.0f);
    float x = (screen.x - bounds.getX()) / bounds.getWidth();
    float y = ((bounds.getBottom() - screen.y) / bounds.getHeight()) * 2.0f - 1.0f;
    return {std::clamp(x, 0.0f, 1.0f), std::clamp(y, -1.0f, 1.0f), 0.0f};
}

int CurveEditor::findNearestPoint(juce::Point<float> screen) const
{
    const auto& points = sequencer.getPoints();
    int nearest = -1;
    float minDist = 15.0f;

    for (int i = 0; i < static_cast<int>(points.size()); ++i) {
        auto screenPt = pointToScreen(points[static_cast<size_t>(i)]);
        float dist = screen.getDistanceFrom(screenPt);
        if (dist < minDist) {
            minDist = dist;
            nearest = i;
        }
    }
    return nearest;
}

void CurveEditor::mouseDown(const juce::MouseEvent& e)
{
    dragIndex = findNearestPoint(e.position);
}

void CurveEditor::mouseDrag(const juce::MouseEvent& e)
{
    if (dragIndex >= 0) {
        auto pt = screenToPoint(e.position);
        sequencer.movePoint(dragIndex, pt.x, pt.y);
        repaint();
    }
}

void CurveEditor::mouseDoubleClick(const juce::MouseEvent& e)
{
    int idx = findNearestPoint(e.position);
    if (idx >= 0) {
        sequencer.removePoint(idx);
    } else {
        auto pt = screenToPoint(e.position);
        sequencer.addPoint(pt.x, pt.y);
    }
    repaint();
}

void CurveEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    g.setColour(Colors::surface);
    g.fillRoundedRectangle(bounds, 8.0f);
    g.setColour(Colors::divider);
    g.drawRoundedRectangle(bounds, 8.0f, 1.0f);

    auto area = bounds.reduced(8.0f);

    g.setColour(Colors::divider);
    g.drawHorizontalLine(static_cast<int>(area.getCentreY()), area.getX(), area.getRight());

    const auto& points = sequencer.getPoints();
    if (points.empty()) return;

    // Curve path
    juce::Path path;
    int resolution = static_cast<int>(area.getWidth());
    for (int px = 0; px <= resolution; ++px) {
        float normX = static_cast<float>(px) / static_cast<float>(resolution);

        float y = 0.0f;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            if (normX >= points[i].x && normX <= points[i + 1].x) {
                float segLen = points[i + 1].x - points[i].x;
                float t = (segLen > 0.0001f) ? (normX - points[i].x) / segLen : 0.0f;
                y = points[i].y + (points[i + 1].y - points[i].y) * t;
                break;
            }
        }
        if (normX <= points.front().x) y = points.front().y;
        if (normX >= points.back().x) y = points.back().y;

        float screenX = area.getX() + normX * area.getWidth();
        float screenY = area.getBottom() - (y + 1.0f) * 0.5f * area.getHeight();

        if (px == 0) path.startNewSubPath(screenX, screenY);
        else path.lineTo(screenX, screenY);
    }

    g.setColour(Colors::highlight);
    g.strokePath(path, juce::PathStrokeType(2.0f));

    for (const auto& pt : points) {
        auto screenPt = pointToScreen(pt);
        g.setColour(Colors::highlight);
        g.fillEllipse(screenPt.x - 5.0f, screenPt.y - 5.0f, 10.0f, 10.0f);
        g.setColour(Colors::surface);
        g.fillEllipse(screenPt.x - 3.0f, screenPt.y - 3.0f, 6.0f, 6.0f);
    }
}

}
