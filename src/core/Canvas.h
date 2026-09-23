#pragma once

#include <cstdint>
#include <vector>

struct Cell {
    char glyph = ' ';
    uint32_t col_fg = 0xFFFFFFFF;
    uint32_t col_bg = 0xFFFFFFFF;
};

class Canvas {
public:
    int width, height;
    std::vector<Cell> cells;

    Canvas(int width = 80, int height = 25) : width(width), height(height), cells(width * height) {}

    void SetCell(int x, int y, char glyph, uint32_t col_fg, uint32_t col_bg) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        cells[y * width + x] = { glyph, col_fg, col_bg };
    };

    const Cell& GetCell(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            static Cell dummy;
            return dummy;
        };
        return cells[y * width + x];
    };
};