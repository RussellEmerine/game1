#pragma once

#include "Mode.hpp"
#include "PPU466.hpp"
#include "Level.hpp"

/*
 * The mode for the select screen.
 *
 * Since it always exists, it does some initialization tasks and owns the ppu.
 * PlayModes and WinModes refer to the ppu that SelectMode owns.
 */
struct SelectMode : Mode {
    SelectMode();
    
    bool handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) override;
    
    void update(float elapsed) override;
    
    void draw(glm::uvec2 const &drawable_size) override;
    
    void set_background();
    
    PPU466 ppu;
    std::vector<Level> levels;
    size_t index;
    bool active;

private:
    void display_level();
};


