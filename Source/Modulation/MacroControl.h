#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace gvnr {

struct MacroMapping {
    std::string targetParamId;
    float minValue = 0.0f;
    float maxValue = 1.0f;
};

class MacroControl {
public:
    void setValue(float v) { value = std::clamp(v, 0.0f, 1.0f); }
    float getValue() const { return value; }

    void addMapping(const MacroMapping& mapping);
    void removeMapping(int index);
    void clearMappings();

    float getMappedValue(const std::string& paramId) const;
    const std::vector<MacroMapping>& getMappings() const { return mappings; }

private:
    float value = 0.0f;
    std::vector<MacroMapping> mappings;
};

}
