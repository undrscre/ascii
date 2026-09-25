#pragma once
// hastily put on together . might have to refactor

enum ToolType {
    Select,
    Brush,
    Picker,
    Shape,
    Bucket,
    Text,
};

struct ToolInfo {
    ToolType type;
    const char* name;
};

static constexpr ToolInfo ToolbarTools[] = {
    { ToolType::Select, "Select" },
    { ToolType::Brush,  "Brush"  },
    { ToolType::Picker, "Picker" },
    { ToolType::Picker, "Shape" },
    { ToolType::Picker, "Bucket" },
    { ToolType::Picker, "Text" },
};