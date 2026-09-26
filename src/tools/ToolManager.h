// prolly need to name this better
#pragma once

#include "Brush.h"
#include "ITool.h"
#include "Picker.h"
#include "Select.h"
#include "Shape.h"
#include "ToolDefinition.h"
#include <memory>
#include <unordered_map>

class ToolManager {
public:
    std::unordered_map<ToolType, std::unique_ptr<ITool>> tools;
    ToolManager() {
        tools[ToolType::SELECT] = std::make_unique<SelectTool>();
        tools[ToolType::BRUSH] = std::make_unique<BrushTool>();
        tools[ToolType::PICKER] = std::make_unique<PickerTool>();
        tools[ToolType::SHAPE] = std::make_unique<ShapeTool>();
    }

    ITool* GetActiveTool(ToolType type) const {
        auto tool = tools.find(type);
        return (tool != tools.end()) ? tool->second.get() : nullptr; 
    }
    
private:
};