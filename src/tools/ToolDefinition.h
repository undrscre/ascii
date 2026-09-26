#pragma once
// hastily put on together . might have to refactor

enum ToolType {
    SELECT,
    BRUSH,
    PICKER,
    SHAPE,
    BUCKET,
    TEXT,
};

struct ToolInfo {
    ToolType type;
    const char* name;
};

static constexpr ToolInfo ToolbarTools[] = {
    { ToolType::SELECT, "Select" },
    { ToolType::BRUSH,  "Brush"  },
    { ToolType::PICKER, "Picker" },
    { ToolType::SHAPE, "Shape" },
    { ToolType::BUCKET, "Bucket" },
    { ToolType::TEXT, "Text" },
};