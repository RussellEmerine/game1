#include "util.hpp"
#include <string>

uint16_t background_tile(uint32_t tile_index, uint32_t palette_index) {
    assert(tile_index < 16 * 16 && palette_index < 8);
    return (uint16_t) (tile_index | (palette_index << 8));
}

uint8_t sprite_attribute(bool background, uint32_t palette_index) {
    assert(palette_index < 8);
    return (uint8_t) (((uint32_t) background << 7) | palette_index);
}

void display_text(PPU466 &ppu, size_t i, size_t j, const std::string &s) {
    assert(i < PPU466::BackgroundHeight && j < PPU466::BackgroundWidth &&
           "tried to display text out of background range");
    for (size_t k = 0; k + j < std::min((size_t) PPU466::BackgroundWidth, s.size() + j); k++) {
        ppu.background[i * PPU466::BackgroundWidth + k + j] = background_tile(s[k], 0);
    }
}
