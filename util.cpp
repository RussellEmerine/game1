#include "util.hpp"

uint16_t background_tile(uint32_t tile_index, uint32_t palette_index) {
    assert(tile_index < 16 * 16 && palette_index < 8);
    return tile_index | (palette_index << 8);
}

uint8_t sprite_attribute(bool background, uint32_t palette_index) {
    assert(palette_index < 8);
    return ((uint8_t) background << 7) | palette_index;
}
