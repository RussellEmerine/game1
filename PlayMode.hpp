#pragma once

#include "Ball.hpp"
#include "PPU466.hpp"
#include "Mode.hpp"
#include "Level.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
    // Initializes the PlayMode with level i
    explicit PlayMode(size_t lvl);
    
    ~PlayMode() override;
    
    //functions called by main loop:
    bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
    
    void update(float elapsed) override;
    
    void draw(glm::uvec2 const &drawable_size) override;
    
    // TODO: add game state
    Level level;
    
    //----- drawing handled by PPU466 -----
    PPU466 ppu;
};
