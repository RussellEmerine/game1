#include <fstream>

#include "Load.hpp"
#include "PlayMode.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"
#include "util.hpp"


typedef std::array<PPU466::Tile, 16 * 16> Atlas;

Load<Atlas> atlas(LoadTagDefault, []() {
    auto *atlas = new Atlas;
    *atlas = {};
    glm::uvec2 size;
    std::vector<glm::u8vec4> data;
    load_png(data_path("atlas.png"), &size, &data, UpperLeftOrigin);
    assert(size == glm::uvec2(16 * 8, 8 * 8) && "atlas.png had the wrong size, should be 16x8 grid of 8x8 tiles");
    for (size_t r = 0; r < 8; ++r) {
        for (size_t c = 0; c < 16; ++c) {
            for (size_t i = 0; i < 8; ++i) {
                for (size_t j = 0; j < 8; ++j) {
                    (*atlas)[16 * r + c].bit0[7 - i] |=
                            (data[(8 * r + i) * 16 * 8 + (8 * c + j)][0] & 1) << j;
                }
            }
        }
    }
    return atlas;
});

PlayMode::PlayMode(size_t lvl) {
    {
        // TODO: make this work on windows
        std::ifstream in(data_path("levels/" + std::to_string(0) + ".lvl"));
        level = Level(in);
    }
    
    ppu.tile_table = *atlas;
    ppu.background_color = level.background_color;
    ppu.palette_table = level.palette_table;
    for (size_t i = 0; i < level.layout.size(); i++) {
        for (size_t j = 0; j < level.layout[i].size(); j++) {
            ppu.background[64 * i + j] = background_tile(level.layout[i][j], 0);
        }
    }
}

PlayMode::~PlayMode() = default;

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    if (evt.type == SDL_KEYDOWN) {
        if (evt.key.keysym.sym == SDLK_q) {
            set_current(nullptr);
        } else {
            return level.handle_event(evt);
        }
    }
    
    return false;
}

void PlayMode::update(float elapsed) {
    level.update(elapsed);
    if (level.tile_at(level.balls[0].pos[0] + 4, level.balls[0].pos[1] + 4) == 16) {
        // TODO: make this a win mode or go to next level
        set_current(nullptr);
    }
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    for (size_t i = 0; i < level.balls.size(); i++) {
        if (0 <= level.balls[i].pos[0] && level.balls[i].pos[0] <= PPU466::ScreenWidth
            && 0 <= level.balls[i].pos[1] && level.balls[i].pos[1] <= PPU466::ScreenHeight) {
            ppu.sprites[i] = {
                    (uint8_t) level.balls[i].pos[0],
                    (uint8_t) level.balls[i].pos[1],
                    static_cast<uint8_t>(i == 0 ? 2 : 3),
                    sprite_attribute(false, level.balls[i].group),
            };
        } else {
            ppu.sprites[i] = {};
        }
    }
    
    ppu.draw(drawable_size);
}
