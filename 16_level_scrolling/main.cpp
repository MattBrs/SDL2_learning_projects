#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_rwops.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_mixer.h"
#include "Texture.hpp"
#include "libs/Timer/Timer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstddef>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include "Constants.hpp"
#include "Dot.hpp"

using namespace texture;

const int WINDOW_WIDTH = constants::WINDOW_WIDTH;
const int WINDOW_HEIGHT = constants::WINDOW_HEIGHT;
const int FRAME_CAP = constants::FRAME_CAP;
const int TICKS_PER_FRAME = constants::TICKS_PER_FRAME;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(Dot &player, Texture &background_texture, Dot &other_dot);
void quit(SDL_Window* &window);
bool load_media(Dot &player, Texture &background_texture, Dot &other_dot);
void save_to_file(Dot &player, std::string path);
void read_from_file(Dot &player, std::string path);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Dot player;
    Texture background_texture;
    Dot other_dot (WINDOW_WIDTH / 3, WINDOW_HEIGHT / 3);

    if (!init(window)) {
        return 1;
    }

    if (!load_media(player, background_texture, other_dot)) {
        return 1;
    }

    read_from_file(player, "player.bin");

    run(player, background_texture, other_dot);
    save_to_file(player, "player.bin");
    quit(window);
    return 0;
}

bool init(SDL_Window* &window) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Player movement", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Error initializing window: %s\n", SDL_GetError());
        return false;
    }

    // we use renderer_accelerated to use GPU rendering, we use renderer_presentvsync to use vsync as frame cap if available
    g_renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (g_renderer == NULL) {
        printf("Error initializing renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);

    int image_flags = IMG_INIT_PNG;
    if (!(IMG_Init(image_flags) & image_flags)) {
        printf("Error initializing sdl_img: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("Error while initializing ttf: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void quit(SDL_Window* &window) {
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool load_media(Dot &player, Texture &background_texture, Dot &other_dot) {
    if (!player.load_texture("./textures/dot.bmp", g_renderer)) {
        printf("Failed to load dot image\n");
        return false;
    }

    if (!other_dot.load_texture("./textures/dot.bmp", g_renderer)) {
        printf("Failed to load dot image\n");
        return false;
    }

    if (!background_texture.load_from_path("./textures/bg.png", g_renderer)) {
        printf("Failed to load dot image\n");
        return false;
    }
    return true;
}

void run(Dot &player, Texture &background_texture, Dot &other_dot) {
    bool quit = false;
    SDL_Event event;

    SDL_Rect camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    Timer frame_cap;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            player.handle_event(event);
        }


        double time_step = frame_cap.get_ticks() / 1000.f;
        player.move(other_dot.get_collider(), time_step);

        camera.x = 
            (player.get_pos_x() + Dot::DOT_WIDTH / 2) - WINDOW_WIDTH / 2;
        camera.y = 
            (player.get_pos_y() + Dot::DOT_HEIGHT / 2) - WINDOW_HEIGHT / 2;

        frame_cap.start();

        if (camera.x < 0) {
            camera.x = 0;
        }

        if (camera.y < 0) {
            camera.y = 0;
        }

        if (camera.x > constants::LEVEL_WIDTH - camera.w) {
            camera.x = constants::LEVEL_WIDTH - camera.w;
        }

        if (camera.y > constants::LEVEL_HEIGHT - camera.h) {
            camera.y = constants::LEVEL_HEIGHT - camera.h;
        }

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        background_texture.render(0, 0, g_renderer, &camera);
        player.render(g_renderer, camera.x, camera.y);
        other_dot.render(g_renderer, camera.x, camera.y);

        SDL_RenderPresent(g_renderer);
    }
}

void save_to_file(Dot &player, std::string path) {
    SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r+b");

    if (file == NULL) {
        file = SDL_RWFromFile(path.c_str(), "w+b");

        if (file == NULL) {
            printf("Could not create file!  %s\n", SDL_GetError());
            return;
        }
    }


    SDL_RWwrite(file, &player, sizeof(player), 1);
    SDL_RWclose(file);
}

void read_from_file(Dot &player, std::string path) {
    SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r+w");

    if (file == NULL) {
        printf("Cannot load %s,  %s\n", path.c_str(), SDL_GetError());
        return;
    }

    Dot temp_player;
    SDL_RWread(file, &temp_player, sizeof(player), 1);
    SDL_RWclose(file);

    player.set_pos_x(temp_player.get_pos_x());
    player.set_pos_y(temp_player.get_pos_y());
}