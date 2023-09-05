//
// Created by Russell Emerine on 9/3/23.
//

#include "Ball.hpp"

Ball::Ball(uint8_t group, const glm::f64vec2 &pos, const glm::f64vec2 &vel) : group(group), pos(pos), vel(vel) {}

glm::f64vec2 Ball::center() const {
    return pos + glm::f64vec2(OFFSET, OFFSET);
}
