#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <stdio.h>
#include <string>

bool init(SDL_Window* &window);         // init sdl and create window
void quit(SDL_Window* &window);         // quit sdl and free memory
void run(SDL_Window* &window);          // execute application: render frames and handle input

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

SDL_Renderer* g_renderer = NULL;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    
    if (!init(window)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    run(window);
    quit(window);

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

    g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == NULL) {
        printf("Error initializing renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);


    int img_flags = IMG_INIT_PNG;
    if (!(IMG_InitFlags(img_flags) & img_flags)) {
        printf("Error itializing png loader: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void quit(SDL_Window* &window) {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);

    g_renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void run(SDL_Window* &window) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        SDL_Rect inner_rect = {WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
        SDL_SetRenderDrawColor(g_renderer, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderFillRect(g_renderer, &inner_rect);

        SDL_Rect outer_rect = {WINDOW_WIDTH/6, WINDOW_HEIGHT/6, WINDOW_WIDTH * 2 / 3, WINDOW_HEIGHT * 2/3};
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0xff, 0x00, 0xff);
        SDL_RenderDrawRect(g_renderer, &outer_rect);

        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0xff, 0xff);
        SDL_RenderDrawLine(g_renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);


        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0x00, 0xff);
        for (int i = 0; i <= WINDOW_HEIGHT; i+=4) {
            SDL_RenderDrawPoint(g_renderer, WINDOW_WIDTH / 2, i);
        }

        SDL_RenderPresent(g_renderer);
    }
}