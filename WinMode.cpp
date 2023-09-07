#include <cstdio>

#include "WinMode.hpp"
#include "util.hpp"

WinMode::WinMode(const std::shared_ptr<Mode> &select, PPU466 &ppu, float time)
        : ppu(ppu), select(select) {
    ppu.sprites = {};
    ppu.background = {};
    // there are ways to do this with stream modifiers, but this is simpler and it's what I know
    char buf[100];
    std::snprintf(buf, 100, "%.3f", time);
    display_text(ppu, 15, 10, std::string(buf));
    display_text(ppu, 10, 10, "Win!");
}

void WinMode::update(float elapsed) {}

void WinMode::draw(const glm::uvec2 &drawable_size) {
    ppu.draw(drawable_size);
}

bool WinMode::handle_event(const SDL_Event &evt, const glm::uvec2 &window_size) {
    if (evt.type == SDL_KEYDOWN && (evt.key.keysym.sym == SDLK_q || evt.key.keysym.sym == SDLK_RETURN)) {
        set_current(select);
    }
    
    return false;
}
