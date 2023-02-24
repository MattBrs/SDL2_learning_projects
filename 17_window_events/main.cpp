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
#include "Window.hpp"
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

Window g_window;
SDL_Renderer* g_renderer = NULL;

bool init();
void run();
void quit();
bool load_media();
void save_to_file(Dot &player, std::string path);
void read_from_file(Dot &player, std::string path);

int main(int argc, char* args[]) {
    if (!init()) {
        return 1;
    }

    if (!load_media()) {
        return 1;
    }

    run();
    quit();
    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
        return false;
    }

    if (!g_window.init()) {
        printf("Error initializing window: %s\n", SDL_GetError());
        return false;
    }

    g_renderer = g_window.create_renderer();
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

void quit() {
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    g_window.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool load_media() {

    return true;
}

void run() {
    bool quit = false;
    SDL_Event event;

    Texture temp_texture;
    temp_texture.load_from_path("textures/window.png", g_renderer);

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            g_window.handle_event(event, g_renderer);
        }

        if (!g_window.is_minimized()) {
            SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(g_renderer);

            temp_texture.render(
                (g_window.get_window_width() - temp_texture.get_witdh()) / 2, 
                (g_window.get_window_heigth() - temp_texture.get_heigth()) / 2,
                g_renderer
            );

            SDL_RenderPresent(g_renderer);            
        }
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