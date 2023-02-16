#include <SDL2/SDL.h>
#include <cstdio>
#include <stdio.h>

const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;


int main(int argc, char* args[]) {

    SDL_Window* window = NULL;          // window
    SDL_Surface* screenSurface = NULL;  // image rendered on window
    
    // init SDL. Catch error and exit program
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { 
        printf("SDL could not initialize! Error: %s \n", SDL_GetError());
        return 1;
    } 

    // create window with name, position, dimensions and flags (show flag)
    window = SDL_CreateWindow("SDL tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    // catch window creation error
    if (window == NULL) {
        printf("There was an error while istantiating the window: %s \n", SDL_GetError());
        return 1;
    }

    // get screenSurface from created window to draw something
    screenSurface = SDL_GetWindowSurface(window);

    // Fill the surface with wite color
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

    // Update the surface on the window
    SDL_UpdateWindowSurface(window);

    // Hack to leave sql window open
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
