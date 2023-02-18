#include "SDL2/SDL_image.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_blendmode.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstddef>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <Texture.hpp>

using namespace texture;

bool init(SDL_Window* &window);                                                         // init sdl and create window
void quit(SDL_Window* &window, Texture &text_texture, TTF_Font* font);                          // quit sdl and free memory
void run(SDL_Window* &window, Texture &text_texture, TTF_Font* font);  // execute application: render frames and handle input
bool load_media(Texture &sprite_sheet_texture, TTF_Font* font);

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;
const int ANIMATION_FRAME_COUNT = 4;

SDL_Renderer* g_renderer = NULL;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    Texture text_texture;
    TTF_Font* font = NULL;

    if (!init(window)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    if (!load_media(text_texture, font)) {
        return 1;
    }

    run(window, text_texture, font);
    quit(window, text_texture, font);

    return 0;
}

bool init(SDL_Window* &window) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "hardware rendering test", 
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

    // renderer to render texture with the gpu intead of the cpu
    g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == NULL) {
        printf("Error initializing renderer: %s\n", SDL_GetError());
        return false;
    }

    // render set default render color to white
    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);

    // init img_sdl_png library so we can load png images
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_InitFlags(img_flags) & img_flags)) {
        printf("Error itializing png loader: %s\n", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("Error initializing ttf: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void quit(SDL_Window* &window, Texture &text_texture, TTF_Font* font) {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);
    text_texture.free();

    g_renderer = NULL;
    window = NULL;
    font = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void run(SDL_Window* &window, Texture &text_texture, TTF_Font* font) {
    bool quit = false;
    SDL_Event event;

    int current_frame = 0;
    double angle = 0.0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    // avoid window closure when application start
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            // listen to quit event to close window when necessary
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: angle -= 10; break;
                    case SDLK_RIGHT: angle += 10; break;
                    case SDLK_w: flip = SDL_FLIP_HORIZONTAL; break;
                    case SDLK_q: flip = SDL_FLIP_NONE; break;
                    case SDLK_e: flip = SDL_FLIP_VERTICAL; break;
                }
            }
        }

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        text_texture.render(
            (WINDOW_WIDTH - text_texture.get_witdh()) / 2, 
            (WINDOW_HEIGHT - text_texture.get_heigth()) / 2, 
            g_renderer
        );

        SDL_RenderPresent(g_renderer);
    }
}

bool load_media(Texture &font_texture,TTF_Font* font) {
    // load media to texture classes
    font = TTF_OpenFont("textures/lazy.ttf", 28);
    if (font == NULL) {
        printf("Error loading font: %s\n", TTF_GetError());
        return false;
    }

    // set all 0 to render black color
    SDL_Color color = {0, 0, 0};

    if (!font_texture.load_from_rendered_text(
        font, 
        "This is a test text of a silly font made by lazyfoo!", 
        color, 
        g_renderer)
    ) {
        printf("Error render font_texture\n");
        return false;
    }

    return true;
}