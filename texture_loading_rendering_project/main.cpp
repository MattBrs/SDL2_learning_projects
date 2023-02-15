#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <stdio.h>
#include <string>

bool init(SDL_Window* &window);                  // init sdl and create window
SDL_Texture* load_texture(std::string path);
bool load_media(SDL_Texture* &texture);
void quit(SDL_Window* &window, SDL_Texture* &texture);                 // quit sdl and free memory
void run(SDL_Window* &window, SDL_Texture* &texture);                  // execute application: render frames and handle input

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

SDL_Renderer* g_renderer = NULL;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    SDL_Texture* current_texture = NULL;                         // image space contained by the window
    
    if (!init(window)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    load_media(current_texture);

    run(window, current_texture);
    quit(window, current_texture);

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

void quit(SDL_Window* &window, SDL_Texture* &texture) {
    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);

    g_renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void run(SDL_Window* &window, SDL_Texture* &texture) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(g_renderer);
        SDL_RenderCopy(g_renderer, texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);
    }
}

SDL_Texture* load_texture(std::string path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* image_surface = NULL;

    image_surface = IMG_Load(path.c_str());

    if (image_surface == NULL) {
        printf("Error loading image: %s\n", IMG_GetError());
        return NULL;
    }

    newTexture = SDL_CreateTextureFromSurface(g_renderer, image_surface);
    if (newTexture == NULL) {
        printf("Error creating texture from surface: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(image_surface);
    return newTexture;
}

bool load_media(SDL_Texture* &texture) {
    texture = load_texture("./images/default_image.bmp");

    if (texture == NULL) {
        printf("Error loading media: %s\n", SDL_GetError());
        return false;
    }

    return true;
}