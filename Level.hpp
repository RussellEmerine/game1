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
    std::string name;
    
    Level();
    
    /*
     * Reads a Level from a .lvl file. Input `name` is the file name without .lvl
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
    explicit Level(const std::string &name);
    
    bool handle_event(SDL_Event const &evt);
    
    void update(float elapsed);
    
    /*
     * Returns the tile at the specified position.
     * Theoretically rounds down for edge cases,
     * but you should add epsilons to ensure the right value.
     * Returns 1 (filled tile) for out-of-bounds locations.
     */
    uint8_t tile_at(glm::f64vec2 pos);
    
    bool operator<(const Level &rhs) const;
    
    bool operator>(const Level &rhs) const;
    
    bool operator<=(const Level &rhs) const;
    
    bool operator>=(const Level &rhs) const;

private:
    /*
     * Moves the ball to the destination, bouncing off any walls.
     * So, the ball does not actually end at the destination, but it
     * moves the same total distance.
     *
     * `dest` is a `Ball` just so `.center()` is available. Only the position
     * is used.
     *
     * The implementation is not "stupid" - it follows the movement to
     * the nearest tile boundary, performs a collision check, places the ball
     * immediately after any collisions or boundary crossings, and recursively
     * calls itself with the new position. This is much more complicated than,
     * for example, assuming all movement distances are small and only checking
     * for one bounce. However, it is conceptually not too hard to understand
     * and should eliminate all game-breaking collision errors, except in rare
     * cases where the corners are hit exactly.
     */
    void move(Ball &ball, Ball dest);
};