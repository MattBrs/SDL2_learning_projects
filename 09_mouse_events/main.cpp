#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <stdio.h>
#include "Button.hpp"

using namespace texture;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 720;
const int BUTTON_COUNT = 4;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(Texture &texture, Button buttons[], SDL_Rect texture_clips[]);
void quit(SDL_Window* &window, Texture &texture);
bool load_media(Texture &texture, SDL_Rect texture_clips[], Button buttons[]);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Texture loaded_texture;
    Button buttons[BUTTON_COUNT];
    SDL_Rect texture_clips[BUTTON_SPRITE_TOTAL];

    if (!init(window)) {
        return 1;
    }

    if (!load_media(loaded_texture, texture_clips, buttons)) {
        return 1;
    }

    run(loaded_texture, buttons, texture_clips);
    quit(window, loaded_texture);
    return 0;
}

bool init(SDL_Window* &window) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing video: %s\n", SDL_GetError());
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

    return true;
}

void quit(SDL_Window* &window, Texture &texture) {
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    texture.free();

    IMG_Quit();
    SDL_Quit();
}

bool load_media(Texture &texture, SDL_Rect texture_clips[], Button buttons[]) {
    if (!texture.load_from_path("textures/button.png", g_renderer)) {
        printf("Error loading button image\n");
        return false;
    }
    
    SDL_Rect button_clip_1 = {0, 0, 300, 200};
    SDL_Rect button_clip_2 = {0, 200, 300, 200};
    SDL_Rect button_clip_3 = {0, 400, 300, 200};
    SDL_Rect button_clip_4 = {0, 600, 300, 200};

    texture_clips[BUTTON_SPRITE_MOUSE_OUT] = button_clip_1;
    texture_clips[BUTTON_SPRITE_MOUSE_OVER] = button_clip_2;
    texture_clips[BUTTON_SPRITE_MOUSE_DOWN] = button_clip_3;
    texture_clips[BUTTON_SPRITE_MOUSE_UP] = button_clip_4;

    buttons[0] = {0, 0, texture_clips};
    buttons[1] = {0, WINDOW_HEIGHT - 300, texture_clips};
    buttons[2] = {WINDOW_WIDTH - 300, WINDOW_HEIGHT - 300, texture_clips};
    buttons[3] = {WINDOW_WIDTH - 300, 0, texture_clips};
    
    return true;
}

void run(Texture &texture, Button buttons[], SDL_Rect texture_clips[]) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            for (int i = 0; i < BUTTON_COUNT; ++i) {
                buttons[i].handle_event(&event);
            }
        }
        
        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);
        
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            buttons[i].render(g_renderer, texture);
        }

        SDL_RenderPresent(g_renderer);
        
    }
}