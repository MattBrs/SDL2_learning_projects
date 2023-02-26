#include "SDL2/SDL.h"
#include "ParticleSystem.hpp"
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
void run(Dot &player);
void quit();
bool load_media(ParticleSystem &particle_system, Dot &player);
void save_to_file(Dot &player, std::string path);
void read_from_file(Dot &player, std::string path);

int main(int argc, char* args[]) {
    ParticleSystem particle_system (
        15,
        30
    );
    Dot player;

    if (!init()) {
        return 1;
    }

    if (!load_media(particle_system, player)) {
        return 1;
    }

    if (!particle_system.init()) {
        printf("Error loading particle system\n");
        return 1;    
    }

    player.set_particle_system(&particle_system);

    run(player);
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

bool load_media(ParticleSystem &particle_system, Dot &player) {
    bool success = true;

    if (!particle_system.add_texture_from_path(
        "textures/blue.bmp", 
        g_renderer
    )) {
        printf("Error loading blue particle image\n");
        success = true;
    }

    if (!particle_system.add_texture_from_path(
        "textures/red.bmp", 
        g_renderer
    )) {
        printf("Error loading red particle image\n");
        success= false;
    }

    if (!particle_system.add_texture_from_path(
        "textures/green.bmp", 
        g_renderer
    )) {
        printf("Error loading green image\n");
        success = false;
    }

    if (!particle_system.set_shimmer_texture_from_path(
        "textures/shimmer.bmp", 
        g_renderer
    )) {
        printf("Error loading shimmer image\n");
        success = false;
    }

    if (!player.load_texture("textures/dot.bmp", g_renderer)) {
        printf("Error loading player image\n");
        success = false;
    }

    return success;
}

void run(Dot &player) {
    bool quit = false;
    SDL_Event event;
    int camera_pos_x = 0;
    int camera_pos_y = 0;


    Timer delta_time;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            g_window.handle_event(event, g_renderer);
            player.handle_event(event);
        }

        if (!g_window.is_minimized()) {
            player.move(delta_time.get_ticks() / 1000.f);
            SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(g_renderer);

            player.render(
                g_renderer, 
                camera_pos_x, 
                camera_pos_y
            ); 
            delta_time.start();

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