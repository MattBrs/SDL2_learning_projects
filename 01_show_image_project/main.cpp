#include <SDL2/SDL.h>
#include <cstdio>
#include <stdio.h>

enum {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAl
};

bool init(SDL_Window* &window, SDL_Surface* &screen_surface);
bool load_white_surface(SDL_Window* &window, SDL_Surface* &screen_surface);
bool load_media(SDL_Window* &window, SDL_Surface* &screen_surface);
void quit(SDL_Window* &window, SDL_Surface* &image_surface);
void stay_up();
void run(SDL_Window* &window, SDL_Surface* &screen_surface, SDL_Surface* &image_surface);

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;          // sdl window
    SDL_Surface* screen_surface = NULL;  // image space contained by the window
    SDL_Surface* image_surface = NULL;   // image space with custom image

    bool init_success= init(window, screen_surface);
    
    if (!init_success) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    // load_white_surface(window, screen_surface);
    bool image_loaded = load_media(window, image_surface);

    if (!image_loaded) {
        printf("there was an error loading the media: %s\n", SDL_GetError());
        return 1;
    }

    run(window, screen_surface, image_surface);

    quit(window, image_surface);
    return 0;
}

bool init(SDL_Window* &window, SDL_Surface* &screen_surface) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow(
        "SDL Tutorial", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        return false;
    }

    screen_surface = SDL_GetWindowSurface(window);

    return true;
}

bool load_white_surface(SDL_Window* &window, SDL_Surface* &screen_surface) {
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xff, 0xff, 0xff));
    SDL_UpdateWindowSurface(window);

    stay_up();

    return true;
}

bool load_media(SDL_Window* &window, SDL_Surface* &image_surface) {
    image_surface = SDL_LoadBMP("../../../Downloads/sunset_wallpaper.bmp");

    if (image_surface == NULL) {
        return false;
    }

    return true;
}

void quit(SDL_Window* &window, SDL_Surface* &image_surface) {
    SDL_FreeSurface(image_surface);
    image_surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void stay_up() {
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
}

void run(SDL_Window* &window, SDL_Surface* &screen_surface, SDL_Surface* &image_surface) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
        SDL_UpdateWindowSurface(window);
    }
}