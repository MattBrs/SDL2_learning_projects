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

using namespace texture;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 720;
const int FRAME_CAP = 60;
const int TICKS_PER_FRAME = 1000/ FRAME_CAP;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(Texture &timer_texture, TTF_Font* &font);
void quit(SDL_Window* &window, Texture &timer_texture, TTF_Font* &font);
bool load_media(TTF_Font* &font);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Texture info_1_texture;
    Texture info_2_texture;
    Texture timer_texture;
    TTF_Font* font = NULL;

    if (!init(window)) {
        return 1;
    }

    if (!load_media(font)) {
        return 1;
    }

    run( timer_texture, font);
    quit(window, timer_texture, font);
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
    g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
    Texture &timer_texture, 
    TTF_Font* &font
) {
    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    timer_texture.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool load_media(TTF_Font* &font) {
    font = TTF_OpenFont("fonts/lazy.ttf", 28);
    if (font == NULL) {
        printf("Error loading font: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void run(Texture &frame_counter_texture, TTF_Font* &font) {
    bool quit = false;
    SDL_Event event;

    SDL_Color text_color = {0, 0, 0, 255};
    std::stringstream timer_text;

    Timer frame_counter;
    frame_counter.start();
    int counted_frames = 0;

    Timer frame_cap;

    while (!quit) {
        frame_cap.start();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        float avg_fps = counted_frames / (frame_counter.get_ticks() / 1000.f);

        timer_text.str("");
        timer_text << avg_fps << "FPS";
        if (!frame_counter_texture.load_from_rendered_text(
            font, 
            timer_text.str(), 
            text_color, 
            g_renderer)
        ) {
            printf("Error rendering text!\n");
        }


        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        frame_counter_texture.render(
            (WINDOW_WIDTH - frame_counter_texture.get_witdh()) / 2, 
            (WINDOW_HEIGHT - frame_counter_texture.get_heigth()) / 2, 
            g_renderer
        );

        SDL_RenderPresent(g_renderer);
        ++counted_frames;

        Uint32 frame_ticks = frame_cap.get_ticks();
        if (frame_ticks < TICKS_PER_FRAME) {
            SDL_Delay(TICKS_PER_FRAME - frame_ticks);
        }
    }
}