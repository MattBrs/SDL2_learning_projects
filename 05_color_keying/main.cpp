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
#include <cstddef>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <Texture.h>


using namespace texture;

bool init(SDL_Window* &window);         // init sdl and create window
void quit(SDL_Window* &window, Texture &actor_texture, Texture &background_texture);         // quit sdl and free memory
void run(SDL_Window* &window, Texture &actor_texture, Texture &background_texture);          // execute application: render frames and handle input
bool load_media(Texture &actor_texture, Texture &background_texture);

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

SDL_Renderer* g_renderer = NULL;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    Texture actor_texture;
    Texture background_texture;

    if (!init(window)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    printf("renderer: %s \n", (g_renderer == NULL) ? "is null":"not null");

    if (!load_media(actor_texture, background_texture)) {
        return 1;
    }

    run(window, actor_texture, background_texture);
    quit(window, actor_texture, background_texture);

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
    g_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

    return true;
}

void quit(SDL_Window* &window, Texture &actor_texture, Texture &background_texture) {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(window);

    g_renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void run(SDL_Window* &window, Texture &actor_texture, Texture &background_texture) {
    bool quit = false;
    SDL_Event event;

    // avoid window closure when application start
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            // listen to quit event to close window when necessary
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);

        background_texture.render(0, 0, g_renderer);
        actor_texture.render(240, 190, g_renderer);

        SDL_RenderPresent(g_renderer);
    }
}

bool load_media(Texture &actor_texture, Texture &background_texture) {
    // load media to texture classes
    bool success = true;

    if (!actor_texture.load_from_path("./textures/foo.png", g_renderer)) {
        printf("Failed to load actor image! \n");
        success = false;
    }

    if (!background_texture.load_from_path("./textures/background.png", g_renderer)) {
        printf("Failed to load background image! \n");
        success = false;
    }

    return success;
}