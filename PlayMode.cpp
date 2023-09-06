#include "PlayMode.hpp"
#include "util.hpp"
#include "WinMode.hpp"

// TODO: still change the title lol

PlayMode::PlayMode(const std::shared_ptr<Mode> &select, PPU466 &ppu, Level level)
        : level(level), select(select), ppu(ppu) {
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
            set_current(select);
        } else {
            return level.handle_event(evt);
        }
    }
    
    return false;
}

void PlayMode::update(float elapsed) {
    level.update(elapsed);
    time += elapsed;
    if (level.tile_at(level.balls[0].pos + glm::f64vec2(4, 4)) == 16) {
        set_current(std::make_shared<WinMode>(select, ppu, time));
    }
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    for (size_t i = 0; i < level.balls.size(); i++) {
        // max with EPSILON to eliminate flashing when at the bottom or left edge
        ppu.sprites[i] = {
                (uint8_t) std::max(level.balls[i].pos[0], EPSILON),
                (uint8_t) std::max(level.balls[i].pos[1], EPSILON),
                static_cast<uint8_t>(i == 0 ? 2 : 3),
                sprite_attribute(false, level.balls[i].group),
        };
    }
    
    ppu.draw(drawable_size);
}
