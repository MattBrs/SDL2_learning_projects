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
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_mixer.h"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <sstream>
#include <stdio.h>

using namespace texture;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 720;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(Texture &info_texture, Texture &timer_texture, TTF_Font* &font);
void quit(SDL_Window* &window, Texture &info_texture, Texture &timer_texture, TTF_Font* &font);
bool load_media(Texture &texture, TTF_Font* &font);
void handle_music(Mix_Music* &music);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Texture info_texture;
    Texture timer_texture;
    TTF_Font* font = NULL;

    if (!init(window)) {
        return 1;
    }

    if (!load_media(info_texture, font)) {
        return 1;
    }

    run(info_texture, timer_texture, font);
    quit(window, info_texture, timer_texture, font);
    return 0;
}

bool init(SDL_Window* &window) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Mouse events", 
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
    g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

void quit(
    SDL_Window* &window, 
    Texture &info_texture, 
    Texture &timer_texture, 
    TTF_Font* &font
) {
    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    info_texture.free();
    timer_texture.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool load_media(Texture &texture, TTF_Font* &font) {
    font = TTF_OpenFont("fonts/lazy.ttf", 28);
    if (font == NULL) {
        printf("Error loading font: %s\n", TTF_GetError());
        return false;
    }

    SDL_Color text_color = {0, 0, 0, 255};
    if (!texture.load_from_rendered_text(font, "Press enter to reset timer", text_color, g_renderer)) {
        printf("Error loading font\n");
        return false;
    }

    return true;
}

void run(Texture &info_texture, Texture &timer_texture, TTF_Font* &font) {
    bool quit = false;
    SDL_Event event;

    SDL_Color text_color = {0, 0, 0, 255};
    Uint32 start_time = 0;
    std::stringstream timer_text;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    start_time = SDL_GetTicks();
                }
            }
        }

        timer_text.str("");
        timer_text << "Milliseconds since start time: " << SDL_GetTicks() - start_time;
        if (!timer_texture.load_from_rendered_text(font, timer_text.str(), text_color, g_renderer)) {
            printf("Error rendering text!\n");
        }

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        info_texture.render(
            (WINDOW_WIDTH - info_texture.get_witdh())/2, 
            0, 
            g_renderer
        );

        timer_texture.render(
            (WINDOW_WIDTH - timer_texture.get_witdh()) / 2, 
            (WINDOW_HEIGHT - timer_texture.get_heigth()) / 2, 
            g_renderer
        );

        SDL_RenderPresent(g_renderer);        
    }
}

