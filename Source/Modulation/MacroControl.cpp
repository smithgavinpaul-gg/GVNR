#include "MacroControl.h"

namespace gvnr {

void MacroControl::addMapping(const MacroMapping& mapping)
{
    mappings.push_back(mapping);
}

void MacroControl::removeMapping(int index)
{
    if (index >= 0 && index < static_cast<int>(mappings.size()))
        mappings.erase(mappings.begin() + index);
}

void MacroControl::clearMappings()
{
    mappings.clear();
}

float MacroControl::getMappedValue(const std::string& paramId) const
{
    for (const auto& mapping : mappings) {
        if (mapping.targetParamId == paramId)
            return mapping.minValue + value * (mapping.maxValue - mapping.minValue);
    }
    return 0.0f;
}

}
