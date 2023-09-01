#pragma once

#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	~PlayMode() override;

	//functions called by main loop:
	bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	void update(float elapsed) override;
	void draw(glm::uvec2 const &drawable_size) override;

    // TODO: add game state
    
	//----- drawing handled by PPU466 -----
	PPU466 ppu;
};
