#include "SDL2/SDL_image.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <stdio.h>
#include <string>


enum {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAl
};

bool init(SDL_Window* &window, SDL_Surface* &screen_surface);                                                       // init sdl and create window
SDL_Surface* load_media_from_path(std::string path, SDL_Surface* screen_surface);                                                                // load specific image from path
void load_media(SDL_Surface* surfaces[], SDL_Surface* screen_surface);                                                                           // load all images to surfaces array
void quit(SDL_Window* &window, SDL_Surface* &image_surface, SDL_Surface* surfaces[]);                               // quit sdl and free memory
void run(SDL_Window* &window, SDL_Surface* &screen_surface, SDL_Surface* &image_surface, SDL_Surface* surfaces[]);  // execute application: render frames and handle input

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

int main (int argc, char* args[]) {
    SDL_Window* window = NULL;                                  // sdl window
    SDL_Surface* screen_surface = NULL;                         // image space contained by the window
    SDL_Surface* current_surface = NULL;                        // image space with custom image
    SDL_Surface* key_press_surfaces[KEY_PRESS_SURFACE_TOTAl];   // images that correspond to a keypress
    
    if (!init(window, screen_surface)) {
        printf("there was an error while initializing sdl: %s \n", SDL_GetError());
        return 1;
    }

    load_media(key_press_surfaces, screen_surface);
    run(window, screen_surface, current_surface, key_press_surfaces);
    quit(window, current_surface, key_press_surfaces);

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

    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image could not initialize: %s\n", IMG_GetError());
        return false;
    }

    screen_surface = SDL_GetWindowSurface(window);

    return true;
}

SDL_Surface* load_media_from_path(std::string path, SDL_Surface* screen_surface) {
    SDL_Surface* optimized_surface = NULL;
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());

    if (loaded_surface == NULL) {
        printf("Error loading media: %s\n", SDL_GetError());
        return loaded_surface;
    }

    // convert the image to screen format
    optimized_surface = SDL_ConvertSurface(loaded_surface, screen_surface->format, 0);

    if (optimized_surface == NULL) {
        printf("Error optimizing image: %s\n", SDL_GetError());
        return loaded_surface;
    }

    SDL_FreeSurface(loaded_surface);

    return optimized_surface;
}

void quit(SDL_Window* &window, SDL_Surface* &image_surface, SDL_Surface* surfaces[]) {
    SDL_FreeSurface(image_surface);
    image_surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void run(SDL_Window* &window, SDL_Surface* &screen_surface, SDL_Surface* &image_surface, SDL_Surface* surfaces[]) {
    bool quit = false;
    SDL_Event event;

    image_surface = surfaces[KEY_PRESS_SURFACE_DEFAULT];

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: 
                        image_surface = surfaces[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN: 
                        image_surface = surfaces[KEY_PRESS_SURFACE_DOWN];
                        break;
                    case SDLK_LEFT: 
                        image_surface = surfaces[KEY_PRESS_SURFACE_LEFT];
                        break;
                    case SDLK_RIGHT:
                        image_surface = surfaces[KEY_PRESS_SURFACE_RIGHT];
                        break;
                    default:
                        image_surface = surfaces[KEY_PRESS_SURFACE_DEFAULT];
                        break;
                }
            }
        }

        //SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = WINDOW_WIDTH;
        stretchRect.h = WINDOW_HEIGHT;
        SDL_BlitScaled(image_surface, NULL, screen_surface, &stretchRect);

        SDL_UpdateWindowSurface(window);
    }
}

void load_media(SDL_Surface* surfaces[], SDL_Surface* screen_surface) {
    surfaces[KEY_PRESS_SURFACE_DEFAULT] = load_media_from_path("./images/default_image.bmp", screen_surface);
    if (surfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("error while loading default image\n");
    }

    surfaces[KEY_PRESS_SURFACE_UP] = load_media_from_path("./images/up_image.bmp", screen_surface);
    if (surfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("error while loading up image\n");
    }

    surfaces[KEY_PRESS_SURFACE_DOWN] = load_media_from_path("./images/down_image.bmp", screen_surface);
    if (surfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("error while loading down image\n");
    }
    
    surfaces[KEY_PRESS_SURFACE_LEFT] = load_media_from_path("./images/left_image.bmp", screen_surface);
    if (surfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("error while loading left image\n");
    }
    
    surfaces[KEY_PRESS_SURFACE_RIGHT] = load_media_from_path("./images/right_image.bmp", screen_surface);
    if (surfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("error while loading right image\n");
    }
}