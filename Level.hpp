#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <SDL.h>
#include "Ball.hpp"
#include "PPU466.hpp"

const double RATE = 0.5, ACC_CAP = 1000.0, VEL_CAP = 10000.0, GRAV = 30000000.0;

// Layout of a level, exactly screen size.
// Each array element is an index into the tile table; all use the same palette
typedef std::array<std::array<uint8_t, 32>, 30> Layout;

/*
 * Representation of a level. Can be loaded in from a .lvl file.
 * Does all the real work of PlayMode.
 */
struct Level {
    glm::u8vec3 background_color{};
    std::array<PPU466::Palette, 8> palette_table{};
    // Level layout, i.e. obstacles and text
    Layout layout{};
    // Ball positions, velocities, and groups, with player ball at index 0
    std::vector<Ball> balls;
    
    Level();
    
    /*
     * Reads a Level from a .lvl file.
     *
     * NOTE: .lvl files
     * A .lvl file is essentially a text file with a particular very simple syntax.
     * The syntax is as follows. All colors are hex values (without prefix),
     * ball groups are integers from 0 to 7 inclusive,
     * and all ball positions and velocities are decimals.
     *
     * line 1: single three-byte color for the background
     * lines 2-9: single four-byte color for the palettes, in order
     * lines 10-39: lines of characters representing tiles
     *      - ' ' represents empty tile (id 0)
     *      - '#' represents filled tile (id 1)
     *      - '@' represents goal (id 16)
     *      - other tiles represent themselves
     *      - lines longer than 32 characters are truncated
     *      - lines shorter than 32 characters are filled with blank tiles
     * lines 40-end: five numbers representing a ball
     *      - as in "group pos.x pos.y vel.x vel.y"
     *      - there should be at least one ball (which will be the player ball)
     *
     * Due to the simplicity and visual effectiveness of the .lvl format,
     * you can easily edit existing levels or create your own levels.
     * Errors in the file format are not handled well yet,
     * but may be detected in the future.
     *
     * TODO: move this documentation into README or similar
     */
    explicit Level(std::istream &in);
    
    bool handle_event(SDL_Event const &evt);
    
    void update(float elapsed);
    
    /*
     * Returns the tile at the specified position.
     * Theoretically rounds down for edge cases,
     * but you should add epsilons to ensure the right value.
     * Returns 1 (filled tile) for out-of-bounds locations.
     */
    uint8_t tile_at(double x, double y);
};