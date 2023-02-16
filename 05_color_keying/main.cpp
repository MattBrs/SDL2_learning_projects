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
void quit(SDL_Window* &window, Texture &actorTexture, Texture &backgroundTexture);         // quit sdl and free memory
void run(SDL_Window* &window, Texture &actorTexture, Texture &backgroundTexture);          // execute application: render frames and handle input
bool loadMedia(Texture &actorTexture, Texture &backgroundTexture);

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

SDL_Renderer* gRenderer = NULL;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    Texture actorTexture (gRenderer);
    Texture backgroundTexture (gRenderer);

    if (!init(window)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    if (!loadMedia(actorTexture, backgroundTexture)) {
        return 1;
    }

    run(window, actorTexture, backgroundTexture);
    quit(window, actorTexture, backgroundTexture);

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
    gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Error initializing renderer: %s\n", SDL_GetError());
        return false;
    }

    // render set default render color to white
    SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

    // init img_sdl_png library so we can load png images
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_InitFlags(img_flags) & img_flags)) {
        printf("Error itializing png loader: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void quit(SDL_Window* &window, Texture &actorTexture, Texture &backgroundTexture) {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(window);

    gRenderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void run(SDL_Window* &window, Texture &actorTexture, Texture &backgroundTexture) {
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

        SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(gRenderer);

        actorTexture.render(240, 950);
        backgroundTexture.render(0, 0);

        SDL_RenderPresent(gRenderer);
    }
}

bool loadMedia(Texture &actorTexture, Texture &backgroundTexture) {
    // load media to texture classes
    bool success = true;

    if (!actorTexture.loadFromPath("./textures/foo.png")) {
        printf("Failed to load actor image! \n");
        success = false;
    }

    if (!backgroundTexture.loadFromPath("./textures/background.png")) {
        printf("Failed to load background image! \n");
        success = false;
    }

    return success;
}