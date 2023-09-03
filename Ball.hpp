#pragma once

#include <glm/glm.hpp>

struct Ball {
    Ball(uint8_t group, const glm::f64vec2 &pos, const glm::f64vec2 &vel);

    uint8_t group;
    glm::f64vec2 pos, vel;
};

