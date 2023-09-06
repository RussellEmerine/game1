#pragma once

#include "PPU466.hpp"

// I manually define EPSILON since numeric_limits<double>::epsilon() doesn't
// work for our numbers, which are on the order of magnitude of 100.
constexpr double EPSILON = 0.0000000001;

uint16_t background_tile(uint32_t tile_index, uint32_t palette_index);

uint8_t sprite_attribute(bool background, uint32_t palette_index);

void display_text(PPU466 &ppu, size_t i, size_t j, const std::string &s);
