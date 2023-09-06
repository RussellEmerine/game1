#include <fstream>
#include <algorithm>

#include "Level.hpp"
#include "util.hpp"
#include "data_path.hpp"

Level::Level() = default;

glm::u8vec3 get_glm_u8vec3(std::istream &in) {
    uint64_t tmp;
    in >> std::hex >> tmp;
    return {
            glm::u8((tmp >> 16) & 0xff),
            glm::u8((tmp >> 8) & 0xff),
            glm::u8(tmp & 0xff)
    };
}

glm::u8vec4 get_glm_u8vec4(std::istream &in) {
    uint64_t tmp;
    in >> std::hex >> tmp;
    return {
            glm::u8((tmp >> 24) & 0xff),
            glm::u8((tmp >> 16) & 0xff),
            glm::u8((tmp >> 8) & 0xff),
            glm::u8(tmp & 0xff)
    };
}

Level::Level(const std::string &name) : name(name) {
    std::ifstream in(data_path("levels/" + name + ".lvl"));
    
    std::string tmp;
    
    background_color = get_glm_u8vec3(in);
    getline(in, tmp);
    
    for (auto &table: palette_table) {
        table = {};
        table[0] = get_glm_u8vec4(in);
        getline(in, tmp);
    }
    
    for (auto &row: layout) {
        std::getline(in, tmp);
        for (size_t i = 0; i < 32; i++) {
            if (i < tmp.size()) {
                switch (tmp[i]) {
                    case ' ':
                        row[i] = 0;
                        break;
                    case '#':
                        row[i] = 1;
                        break;
                    case '@':
                        row[i] = 16;
                        break;
                    default:
                        row[i] = (uint8_t) tmp[i];
                }
            } else {
                row[i] = 0;
            }
        }
    }
    // PPU466 goes bottom to top but file goes top to bottom, so reverse the order
    std::reverse(layout.begin(), layout.end());
    
    uint64_t group;
    glm::f64 posx, posy, velx, vely;
    while (in >> group >> posx >> posy >> velx >> vely) {
        balls.emplace_back(
                (uint8_t) group,
                glm::f64vec2(posx, posy),
                glm::f64vec2(velx, vely)
        );
    }
}

bool Level::handle_event(const SDL_Event &evt) {
    if (evt.type == SDL_KEYDOWN) {
        // TODO: clean this up
        switch (evt.key.keysym.sym) {
            case SDLK_1:
                balls[0].group = 0;
                break;
            case SDLK_2:
                balls[0].group = 1;
                break;
            case SDLK_3:
                balls[0].group = 2;
                break;
            case SDLK_4:
                balls[0].group = 3;
                break;
            case SDLK_5:
                balls[0].group = 4;
                break;
            case SDLK_6:
                balls[0].group = 5;
                break;
            case SDLK_7:
                balls[0].group = 6;
                break;
            case SDLK_8:
                balls[0].group = 7;
                break;
        }
    }
    
    return false;
}

void Level::update(float elapsed) {
    // find acceleration and update velocities
    for (size_t i = 0; i < balls.size(); i++) {
        for (size_t j = 0; j < balls.size(); j++) {
            double r = glm::length(balls[j].pos - balls[i].pos);
            if (i == j || balls[i].group != balls[j].group || r < 4.0) continue;
            glm::f64vec2 acc = GRAV * glm::normalize(balls[j].pos - balls[i].pos) / (r * r);
            // hard cap the acceleration
            if (glm::length(acc) > ACC_CAP) {
                acc = glm::normalize(acc) * ACC_CAP;
            }
            balls[i].vel += (double) elapsed * RATE * acc;
        }
    }
    
    // update positions
    // TODO: collision
    for (Ball &ball: balls) {
        // hard cap the velocity
        if (glm::length(ball.vel) > VEL_CAP) {
            ball.vel = glm::normalize(ball.vel) * VEL_CAP;
        }
        // move the ball to its destination according to the velocity
        // group 0 and ball.vel are ignored
        move(ball, Ball(0, ball.pos + (double) elapsed * RATE * ball.vel, ball.vel));
    }
}

void Level::move(Ball &ball, Ball dest) {
    // how far until a tile boundary...
    glm::f64 dist = 100;
    // and which way the first boundary is.
    // yes yes, enum would be better style
    char dir = ' ';
    glm::f64 bound = 0;
    
    // check if movements collide
    // there might be a way to make this into a nice for loop but that's too clever
    {
        // x means edge of the ball
        glm::f64 x = ball.center()[0] - Ball::R;
        // b means tile boundary
        // this operation doesn't work if there are negatives,
        // but balls shouldn't have any part negative.
        glm::f64 b = std::floor(x / 8) * 8;
        // y means edge of the ball when it reaches the destination
        glm::f64 y = dest.center()[0] - Ball::R;
        if (y + EPSILON < b) {
            // distance travelled
            glm::f64 d = (x - b) * glm::length(dest.pos - ball.pos) / std::abs(dest.pos[0] - ball.pos[0]);
            if (d < dist) {
                dist = d;
                dir = 'L';
                bound = b;
            }
        }
    }
    {
        glm::f64 x = ball.center()[0] + Ball::R;
        glm::f64 b = std::ceil(x / 8) * 8;
        glm::f64 y = dest.center()[0] + Ball::R;
        if (y - EPSILON > b) {
            glm::f64 d = (b - x) * glm::length(dest.pos - ball.pos) / std::abs(dest.pos[0] - ball.pos[0]);
            if (d < dist) {
                dist = d;
                dir = 'R';
                bound = b;
            }
        }
    }
    {
        glm::f64 x = ball.center()[1] - Ball::R;
        glm::f64 b = std::floor(x / 8) * 8;
        glm::f64 y = dest.center()[1] - Ball::R;
        if (y + EPSILON < b) {
            glm::f64 d = (x - b) * glm::length(dest.pos - ball.pos) / std::abs(dest.pos[1] - ball.pos[1]);
            if (d < dist) {
                dist = d;
                dir = 'D';
                bound = b;
            }
        }
    }
    {
        glm::f64 x = ball.center()[1] + Ball::R;
        glm::f64 b = std::ceil(x / 8) * 8;
        glm::f64 y = dest.center()[1] + Ball::R;
        if (y - EPSILON > b) {
            glm::f64 d = (b - x) * glm::length(dest.pos - ball.pos) / std::abs(dest.pos[1] - ball.pos[1]);
            if (d < dist) {
                dist = d;
                dir = 'U';
                bound = b;
            }
        }
    }
    
    switch (dir) {
        case ' ':
            // no boundaries, go directly to dest
            ball.pos = dest.pos;
            break;
        case 'L':
            // move to the boundary
            ball.pos += dist * glm::normalize(dest.pos - ball.pos);
            // if there is collision
            if (tile_at(ball.center() + glm::f64vec2(-Ball::R - EPSILON, -Ball::R)) == 1
                || tile_at(ball.center() + glm::f64vec2(-Ball::R - EPSILON, Ball::R)) == 1) {
                // make sure it's on the correct side
                ball.pos[0] += EPSILON;
                // reflect direction
                ball.vel[0] *= -1;
                // clumsy but correct
                // the new center should be at bound + (bound - (dest.center()[0] - Ball::R))
                // so manually adjust it back
                dest.pos[0] = bound + (bound - (dest.center()[0] - Ball::R)) + Ball::R - Ball::OFFSET;
            } else {
                // no collision so no changes to dest or vel
                // make sure it's on the correct side
                ball.pos[0] -= EPSILON;
            }
            move(ball, dest);
            break;
        case 'R':
            ball.pos += dist * glm::normalize(dest.pos - ball.pos);
            if (tile_at(ball.center() + glm::f64vec2(Ball::R + EPSILON, -Ball::R)) == 1
                || tile_at(ball.center() + glm::f64vec2(Ball::R + EPSILON, Ball::R)) == 1) {
                ball.pos[0] -= EPSILON;
                ball.vel[0] *= -1;
                dest.pos[0] = bound - ((dest.center()[0] + Ball::R) - bound) - Ball::R - Ball::OFFSET;
            } else {
                ball.pos[0] += EPSILON;
            }
            move(ball, dest);
            break;
        case 'D':
            ball.pos += dist * glm::normalize(dest.pos - ball.pos);
            if (tile_at(ball.center() + glm::f64vec2(Ball::R, -Ball::R - EPSILON)) == 1
                || tile_at(ball.center() + glm::f64vec2(-Ball::R, -Ball::R - EPSILON)) == 1) {
                ball.pos[1] += EPSILON;
                ball.vel[1] *= -1;
                dest.pos[1] = bound + (bound - (dest.center()[1] - Ball::R)) + Ball::R - Ball::OFFSET;
            } else {
                ball.pos[1] -= EPSILON;
            }
            move(ball, dest);
            break;
        case 'U':
            ball.pos += dist * glm::normalize(dest.pos - ball.pos);
            if (tile_at(ball.center() + glm::f64vec2(Ball::R, Ball::R + EPSILON)) == 1
                || tile_at(ball.center() + glm::f64vec2(-Ball::R, Ball::R + EPSILON)) == 1) {
                ball.pos[1] -= EPSILON;
                ball.vel[1] *= -1;
                dest.pos[1] = bound - ((dest.center()[1] + Ball::R) - bound) - Ball::R - Ball::OFFSET;
            } else {
                ball.pos[1] += EPSILON;
            }
            move(ball, dest);
            break;
        default:
            assert(false && "invalid dir char, should not happen");
    }
}

uint8_t Level::tile_at(glm::f64vec2 pos) {
    if (pos[0] < 0 || pos[1] < 0) {
        return 1;
    }
    auto j = (size_t) (pos[0] / 8), i = (size_t) (pos[1] / 8);
    if (i >= layout.size() || j >= layout[i].size()) {
        return 1;
    }
    return layout[i][j];
}

bool Level::operator<(const Level &rhs) const {
    return name < rhs.name;
}

bool Level::operator>(const Level &rhs) const {
    return rhs < *this;
}

bool Level::operator<=(const Level &rhs) const {
    return !(rhs < *this);
}

bool Level::operator>=(const Level &rhs) const {
    return !(*this < rhs);
}


