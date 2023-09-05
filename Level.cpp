#include "Level.hpp"

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

Level::Level(std::istream &in) {
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
            case SDLK_0:
                balls[0].group = 0;
                break;
            case SDLK_1:
                balls[0].group = 1;
                break;
            case SDLK_2:
                balls[0].group = 2;
                break;
            case SDLK_3:
                balls[0].group = 3;
                break;
            case SDLK_4:
                balls[0].group = 4;
                break;
            case SDLK_5:
                balls[0].group = 5;
                break;
            case SDLK_6:
                balls[0].group = 6;
                break;
            case SDLK_7:
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
        ball.pos += (double) elapsed * RATE * ball.vel;
    }
}

uint8_t Level::tile_at(double x, double y) {
    if (x < 0 || y < 0) {
        return 1;
    }
    auto j = (size_t) (x / 8), i = (size_t) (y / 8);
    if (i >= layout.size() || j >= layout[i].size()) {
        return 1;
    }
    return layout[i][j];
}


