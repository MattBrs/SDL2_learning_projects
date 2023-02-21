#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
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
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include "Constants.hpp"
#include "Dot.hpp"

using namespace texture;

const int WINDOW_WIDTH = constants::WINDOW_WIDTH;
const int WINDOW_HEIGHT = constants::WINDOW_HEIGHT;
const int FRAME_CAP = constants::FRAME_CAP;
const int TICKS_PER_FRAME = constants::TICKS_PER_FRAME;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(Dot &player);
void quit(SDL_Window* &window);
bool load_media(Dot &player);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Dot player;

    if (!init(window)) {
        return 1;
    }

    if (!load_media(player)) {
        return 1;
    }

    run(player);
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

bool load_media(Dot &player) {
    if (!player.load_texture("./textures/dot.bmp", g_renderer)) {
        printf("Failed to load dot image\n");
        return false;
    }

    return true;
}

void run(Dot &player) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            player.handle_event(event);
        }

        player.move();

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        player.render(g_renderer);

        SDL_RenderPresent(g_renderer);
    }
}