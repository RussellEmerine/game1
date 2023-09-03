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
    ppu.palette_table[1] = {
            glm::u8vec4(0x60, 0x40, 0x00, 0xaf),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    // NOTE: can't make this work if it's in a Load<>, for some reason it loads mostly right but with missing pixels
    // TODO: make sure this is on the eventual startup mode
    {
        ppu.tile_table = {};
        glm::uvec2 size;
        std::vector<glm::u8vec4> data;
        load_png(data_path("atlas.png"), &size, &data, UpperLeftOrigin);
        assert(size == glm::uvec2(16 * 8, 8 * 8) && "atlas.png had the wrong size, should be 16x8 grid of 8x8 tiles");
        for (size_t r = 0; r < 8; ++r) {
            for (size_t c = 0; c < 16; ++c) {
                for (size_t i = 0; i < 8; ++i) {
                    for (size_t j = 0; j < 8; ++j) {
                        ppu.tile_table[16 * r + c].bit0[7 - i] |=
                                (data[(8 * r + i) * 16 * 8 + (8 * c + j)][0] & 1) << j;
                    }
                }
            }
        }
    }
    
    ppu.background_color = glm::u8vec4(0xff, 0xdf, 0xff, 0xff);
    ppu.background = {};
    
    balls = {
            Ball(0, glm::f64vec2(10.0, 10.0), glm::f64vec2(0.0, 0.0)),
            Ball(1, glm::f64vec2(200.0, 100.0), glm::f64vec2(0.0, 0.0)),
            Ball(1, glm::f64vec2(150.0, 20.0), glm::f64vec2(0.0, 0.0)),
    };

//    for (uint32_t i = 0; i < 16; ++i) {
//        for (uint32_t j = 0; j < 16; ++j) {
//            ppu.background[i + PPU466::BackgroundWidth * j] = background_tile(i + 16 * j, 0);
//        }
//    }
}

PlayMode::~PlayMode() = default;

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    if (evt.type == SDL_KEYDOWN) {
        // TODO: clean this up
        if (evt.key.keysym.sym == SDLK_q) {
            set_current(nullptr);
        } else if (evt.key.keysym.sym == SDLK_1) {
            balls[0].group = 1;
        }
    }
    
    return false;
}

void PlayMode::update(float elapsed) {
    // TODO: update game state
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    // TODO: draw based on game state
    for (size_t i = 0; i < balls.size(); i++) {
        ppu.sprites[i] = {
                (uint8_t) balls[i].pos[0],
                (uint8_t) balls[i].pos[1],
                static_cast<uint8_t>(i == 0 ? 2 : 3),
                sprite_attribute(false, balls[i].group),
        };
    }
    
    ppu.draw(drawable_size);
}
