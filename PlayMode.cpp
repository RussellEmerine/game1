#include "PlayMode.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"
#include "util.hpp"

const double RATE = 0.5, ACC_CAP = 1000.0, VEL_CAP = 10000.0;

PlayMode::PlayMode() {
    // TODO: more palettes
    ppu.palette_table[0] = {
            glm::u8vec4(0x00, 0x00, 0x00, 0xff),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[1] = {
            glm::u8vec4(0x60, 0x40, 0x00, 0xaf),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[2] = {
            glm::u8vec4(0x60, 0x40, 0xff, 0xbf),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[3] = {
            glm::u8vec4(0x60, 0x00, 0xf0, 0x03),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[4] = {
            glm::u8vec4(0x00, 0x40, 0x00, 0xef),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[5] = {
            glm::u8vec4(0x00, 0xe0, 0x00, 0xff),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[6] = {
            glm::u8vec4(0x00, 0xe0, 0xe0, 0xdf),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.palette_table[7] = {
            glm::u8vec4(0x60, 0x40, 0xf0, 0xbf),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    // NOTE: can't make this work if it's in a Load<>, for some reason it loads mostly right but with missing pixels
    // TODO: make sure this is on the eventual startup mode
    {
        ppu.tile_table = {};
        glm::uvec2 size;
        std::vector<glm::u8vec4> data;
        load_png(data_path("atlas.png"), &size, &data, UpperLeftOrigin);
        assert(size == glm::uvec2(16 * 8, 8 * 8) && "atlas.png had the wrong size, should be 16x8 grid of 8x8 tiles");
        for (size_t r = 0; r < 8; ++r) {
            for (size_t c = 0; c < 16; ++c) {
                for (size_t i = 0; i < 8; ++i) {
                    for (size_t j = 0; j < 8; ++j) {
                        ppu.tile_table[16 * r + c].bit0[7 - i] |=
                                (data[(8 * r + i) * 16 * 8 + (8 * c + j)][0] & 1) << j;
                    }
                }
            }
        }
    }
    
    ppu.background_color = glm::u8vec4(0xff, 0xdf, 0xff, 0xff);
    ppu.background = {};
    
    // TODO: load balls from asset
    balls = {
            Ball(0, glm::f64vec2(140.0, 200.0), glm::f64vec2(0.0, 0.0)),
            Ball(1, glm::f64vec2(100.0, 100.0), glm::f64vec2(0.0, 0.0)),
            Ball(1, glm::f64vec2(150.0, 120.0), glm::f64vec2(0.0, 0.0)),
    };
}

PlayMode::~PlayMode() = default;

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
    if (evt.type == SDL_KEYDOWN) {
        // TODO: clean this up
        switch (evt.key.keysym.sym) {
            case SDLK_q:
                set_current(nullptr);
                break;
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

void PlayMode::update(float elapsed) {
    // TODO: update game state
    // find acceleration and update velocities
    for (size_t i = 0; i < balls.size(); i++) {
        for (size_t j = 0; j < balls.size(); j++) {
            double r = glm::length(balls[j].pos - balls[i].pos);
            if (i == j || balls[i].group != balls[j].group || r < 4.0) continue;
            glm::f64vec2 acc = 10000000.0 * glm::normalize(balls[j].pos - balls[i].pos) / (r * r);
            // hard cap the acceleration
            if (glm::length(acc) > ACC_CAP) {
                acc = glm::normalize(acc) * ACC_CAP;
            }
            balls[i].vel += (double) elapsed * RATE * acc;
        }
    }
    
    // update positions
    for (Ball &ball: balls) {
        // hard cap the velocity
        if (glm::length(ball.vel) > VEL_CAP) {
            ball.vel = glm::normalize(ball.vel) * VEL_CAP;
        }
        ball.pos += (double) elapsed * RATE * ball.vel;
    }
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    // TODO: draw based on game state
    for (size_t i = 0; i < balls.size(); i++) {
        if (0 <= balls[i].pos[0] && balls[i].pos[0] <= PPU466::ScreenWidth
            && 0 <= balls[i].pos[1] && balls[i].pos[1] <= PPU466::ScreenHeight) {
            ppu.sprites[i] = {
                    (uint8_t) balls[i].pos[0],
                    (uint8_t) balls[i].pos[1],
                    static_cast<uint8_t>(i == 0 ? 2 : 3),
                    sprite_attribute(false, balls[i].group),
            };
        } else {
            ppu.sprites[i] = {};
        }
    }
    
    ppu.draw(drawable_size);
}
