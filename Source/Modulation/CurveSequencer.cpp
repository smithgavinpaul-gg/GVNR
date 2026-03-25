#include "CurveSequencer.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace gvnr {

void CurveSequencer::prepare(double sr, int)
{
    sampleRate = sr;
    setRate(1.0f);
    if (points.empty()) {
        points.push_back({0.0f, 0.0f, 0.0f});
        points.push_back({0.5f, 1.0f, 0.0f});
        points.push_back({1.0f, 0.0f, 0.0f});
    }
}

void CurveSequencer::reset()
{
    phase = 0.0f;
    currentValue = 0.0f;
}

void CurveSequencer::setRate(float hz)
{
    rateHz = std::max(hz, 0.001f);
    phaseIncrement = rateHz / static_cast<float>(sampleRate);
}

void CurveSequencer::setSync(bool s)
{
    synced = s;
}

void CurveSequencer::setPoints(const std::vector<CurvePoint>& newPoints)
{
    points = newPoints;
    std::sort(points.begin(), points.end(),
              [](const CurvePoint& a, const CurvePoint& b) { return a.x < b.x; });
}

void CurveSequencer::addPoint(float x, float y, float curve)
{
    points.push_back({x, y, curve});
    std::sort(points.begin(), points.end(),
              [](const CurvePoint& a, const CurvePoint& b) { return a.x < b.x; });
}

void CurveSequencer::removePoint(int index)
{
    if (index >= 0 && index < static_cast<int>(points.size()) && points.size() > 2)
        points.erase(points.begin() + index);
}

void CurveSequencer::movePoint(int index, float x, float y)
{
    if (index >= 0 && index < static_cast<int>(points.size())) {
        points[static_cast<size_t>(index)].x = std::clamp(x, 0.0f, 1.0f);
        points[static_cast<size_t>(index)].y = std::clamp(y, -1.0f, 1.0f);
        std::sort(points.begin(), points.end(),
                  [](const CurvePoint& a, const CurvePoint& b) { return a.x < b.x; });
    }
}

void CurveSequencer::clearPoints()
{
    points.clear();
    points.push_back({0.0f, 0.0f, 0.0f});
    points.push_back({1.0f, 0.0f, 0.0f});
}

void CurveSequencer::randomize()
{
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<float> yDist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> curveDist(-1.0f, 1.0f);
    std::uniform_int_distribution<int> countDist(3, 8);

    points.clear();
    int count = countDist(gen);
    for (int i = 0; i < count; ++i) {
        float x = static_cast<float>(i) / static_cast<float>(count - 1);
        points.push_back({x, yDist(gen), curveDist(gen)});
    }
}

float CurveSequencer::interpolate(float y1, float y2, float t, float curve) const
{
    if (std::abs(curve) < 0.001f)
        return y1 + (y2 - y1) * t;

    float shaped = (curve > 0.0f)
                   ? std::pow(t, 1.0f + curve * 3.0f)
                   : 1.0f - std::pow(1.0f - t, 1.0f - curve * 3.0f);

    return y1 + (y2 - y1) * shaped;
}

float CurveSequencer::evaluateAtPhase(float p) const
{
    if (points.empty()) return 0.0f;
    if (points.size() == 1) return points[0].y;

    p = std::clamp(p, 0.0f, 1.0f);

    if (p <= points.front().x) return points.front().y;
    if (p >= points.back().x) return points.back().y;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        if (p >= points[i].x && p <= points[i + 1].x) {
            float segLength = points[i + 1].x - points[i].x;
            if (segLength < 0.0001f) return points[i].y;
            float t = (p - points[i].x) / segLength;
            return interpolate(points[i].y, points[i + 1].y, t, points[i].curve);
        }
    }

    return points.back().y;
}

void CurveSequencer::process(int numSamples, double)
{
    for (int i = 0; i < numSamples; ++i) {
        currentValue = evaluateAtPhase(phase);
        phase += phaseIncrement;
        if (phase >= 1.0f)
            phase -= 1.0f;
    }
}

}
