#pragma once

#include "PPU466.hpp"

uint16_t background_tile(uint32_t tile_index, uint32_t palette_index);

uint8_t sprite_attribute(bool background, uint32_t palette_index);
