// prolly need to name this better
#pragma once

#include "Brush.h"
#include "ITool.h"
#include "ToolDefinition.h"
#include <memory>
#include <unordered_map>

class ToolManager {
public:
    std::unordered_map<ToolType, std::unique_ptr<ITool>> tools;
    ToolManager() {
        tools[ToolType::Brush] = std::make_unique<BrushTool>();
    }

    ITool* GetActiveTool(ToolType type) {
        auto tool = tools.find(type);
        return (tool != tools.end()) ? tool->second.get() : nullptr; 
    }
    
private:
};