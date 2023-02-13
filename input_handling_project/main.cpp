#include <SDL2/SDL.h>
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

bool init(SDL_Window* &window, SDL_Surface* &screen_surface);
SDL_Surface* load_media_from_path(std::string path);
void load_media(SDL_Surface* surfaces[]);
void quit(SDL_Window* &window, SDL_Surface* &image_surface, SDL_Surface* surfaces[]);
void run(SDL_Window* &window, SDL_Surface* &screen_surface, SDL_Surface* &image_surface, SDL_Surface* surfaces[]);

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

    load_media(key_press_surfaces);
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

    screen_surface = SDL_GetWindowSurface(window);

    return true;
}

SDL_Surface* load_media_from_path(std::string path) {
    SDL_Surface* current_surface = SDL_LoadBMP(path.c_str());

    if (current_surface == NULL) {
        printf("Error loading media: %s\n", SDL_GetError());
    }

    return current_surface;
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

        SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
        SDL_UpdateWindowSurface(window);
    }
}

void load_media(SDL_Surface* surfaces[]) {
    surfaces[KEY_PRESS_SURFACE_DEFAULT] = load_media_from_path("./images/default_image.bmp");
    if (surfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("error while loading default image\n");
    }

    surfaces[KEY_PRESS_SURFACE_UP] = load_media_from_path("./images/up_image.bmp");
    if (surfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("error while loading up image\n");
    }

    surfaces[KEY_PRESS_SURFACE_DOWN] = load_media_from_path("./images/down_image.bmp");
    if (surfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("error while loading down image\n");
    }
    
    surfaces[KEY_PRESS_SURFACE_LEFT] = load_media_from_path("./images/left_image.bmp");
    if (surfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("error while loading left image\n");
    }
    
    surfaces[KEY_PRESS_SURFACE_RIGHT] = load_media_from_path("./images/right_image.bmp");
    if (surfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("error while loading right image\n");
    }
}