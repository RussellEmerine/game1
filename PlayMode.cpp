#include <iostream>
#include "PlayMode.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"
#include "util.hpp"

PlayMode::PlayMode() {
    // TODO: more palettes
    ppu.palette_table[0] = {
            glm::u8vec4(0x00, 0x00, 0x00, 0xff),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    
    // TODO: move into a Load<>
    {
        ppu.tile_table = {};
        glm::uvec2 size;
        std::vector<glm::u8vec4> data;
        load_png(data_path("tiles.png"), &size, &data, UpperLeftOrigin);
        assert(size == glm::uvec2(16 * 8, 8 * 8) && "tiles.png had the wrong size, should be 16x8 grid of 8x8 tiles");
        for (size_t r = 0; r < 8; ++r) {
            for (size_t c = 0; c < 16; ++c) {
                for (size_t i = 0; i < 8; ++i) {
                    for (size_t j = 0; j < 8; ++j) {
                        ppu.tile_table[16 * r + c].bit0[7 - i] |= (data[(8 * r + i) * 16 * 8 + (8 * c + j)][0] & 1) << j;
                    }
                }
            }
        }
    }
    
    ppu.background_color = glm::u8vec4(0xff, 0xff, 0xff, 0xff);
    
    for (uint32_t i = 0; i < PPU466::BackgroundWidth; ++i) {
        for (uint32_t j = 0; j < PPU466::BackgroundHeight; ++j) {
            ppu.background[i + PPU466::BackgroundWidth * j] = background_tile(0, 0);
        }
    }
    
    for (uint32_t i = 0; i < 16; ++i) {
        for (uint32_t j = 0; j < 16; ++j) {
            ppu.background[i + PPU466::BackgroundWidth * j] = background_tile(i + 16 * j, 0);
        }
    }
}

PlayMode::~PlayMode() = default;

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_q) {
            set_current(nullptr);
        }
    }
    
    return false;
}

void PlayMode::update(float elapsed) {
    // TODO: update game state
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    // TODO: draw based on game state
    ppu.draw(drawable_size);
}
