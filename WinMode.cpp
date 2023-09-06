#include "WinMode.hpp"
#include "util.hpp"

WinMode::WinMode(const std::shared_ptr<Mode> &select, PPU466 &ppu, float time)
        : ppu(ppu), select(select) {
    ppu.sprites = {};
    ppu.background = {};
    // TODO: make this display time to 3 digits or something
    display_text(ppu, 15, 10, std::to_string(time));
    display_text(ppu, 10, 10, "Win!");
}

void WinMode::update(float elapsed) {}

void WinMode::draw(const glm::uvec2 &drawable_size) {
    ppu.draw(drawable_size);
}

bool WinMode::handle_event(const SDL_Event &evt, const glm::uvec2 &window_size) {
    if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_q) {
        set_current(select);
    }
    
    return false;
}
