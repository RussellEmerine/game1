#pragma once

#include "Ball.hpp"
#include "PPU466.hpp"
#include "Mode.hpp"
#include "SelectMode.hpp"
#include "Level.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
    // Initializes the PlayMode with reference to the select mode and the given level
    explicit PlayMode(const std::shared_ptr<Mode> &select, PPU466 &ppu, Level level);
    
    ~PlayMode() override;
    
    // functions called by main loop:
    bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
    
    void update(float elapsed) override;
    
    void draw(glm::uvec2 const &drawable_size) override;
    
    Level level;
    float time = 0;
    std::shared_ptr<Mode> select;
    PPU466 &ppu;
};
