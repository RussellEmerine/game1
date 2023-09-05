#pragma once

#include <glm/glm.hpp>

/*
 * Ball structure, containing the group, position, and velocity.
 */
struct Ball {
    // Balls sprites have visual radius 4, but collision will have radius 3.7.
    constexpr static glm::f64 R = 3.7;
    constexpr static glm::f64 OFFSET = 4;
    
    uint8_t group;
    glm::f64vec2 pos, vel;
    
    Ball(uint8_t group, const glm::f64vec2 &pos, const glm::f64vec2 &vel);
    
    [[nodiscard]] glm::f64vec2 center() const;
};
