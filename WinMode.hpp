#pragma once

#include "Mode.hpp"
#include "SelectMode.hpp"

struct WinMode : Mode {
    PPU466 &ppu;
    std::shared_ptr<Mode> select;
    
    WinMode(const std::shared_ptr<Mode> &select, PPU466 &ppu, float time);
    
    bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) override;
    
    void update(float elapsed) override;
    
    void draw(glm::uvec2 const &drawable_size) override;
};


