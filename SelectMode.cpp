#include <filesystem>
#include <algorithm>

#include "SelectMode.hpp"
#include "PlayMode.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"
#include "util.hpp"

bool SelectMode::handle_event(const SDL_Event &evt, const glm::uvec2 &window_size) {
    if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
            case SDLK_q:
                set_current(nullptr);
                break;
            case SDLK_UP:
            case SDLK_LEFT:
                index = (index + levels.size() - 1) % levels.size();
                set_background();
                break;
            case SDLK_DOWN:
            case SDLK_RIGHT:
                index = (index + 1) % levels.size();
                set_background();
                break;
            case SDLK_RETURN:
                active = false;
                // this copies the level, which is intended
                set_current(std::make_shared<PlayMode>(shared_from_this(), ppu, levels[index]));
        }
    }
    
    return false;
}

void SelectMode::update(float elapsed) {
    if (!active) {
        set_background();
        active = true;
    }
}

void SelectMode::draw(const glm::uvec2 &drawable_size) {
    // all the work's already done
    ppu.draw(drawable_size);
}

SelectMode::SelectMode() : index(0), active(false) {
    // initialize tile table (only needs to be done once, since all modes use the same tiles)
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
    
    // look through the levels folder and initialize the level list
    for (auto const &f: std::filesystem::directory_iterator(data_path("levels"))) {
        if (f.is_regular_file() && f.path().extension().string() == ".lvl") {
            levels.emplace_back(f.path().stem().string());
        }
    }
    
    // TODO: document support of only ASCII filenames
    assert(!levels.empty() && "there are no levels in the levels folder, put some there are run again");
    
    // this is slow because of lots of moving things around but it doesn't matter unless you
    // have thousands of levels.
    std::sort(levels.begin(), levels.end());
    // other ppu things initialized when update realizes it isn't active
}

void SelectMode::set_background() {
    // simple black-and-white theme
    ppu.background_color = glm::u8vec3(0xff, 0xff, 0xff);
    ppu.palette_table = {};
    ppu.palette_table[0] = {
            glm::u8vec4(0x00, 0x00, 0x00, 0xff),
            glm::u8vec4(0x00, 0xff, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
            glm::u8vec4(0x00, 0x00, 0x00, 0x00),
    };
    ppu.sprites = {};
    display_level();
}

void SelectMode::display_level() {
    ppu.background = {};
    display_text(ppu, 10, 10, "Play!");
    display_text(ppu, 15, 10, levels[index].name);
}
