#include "SDL2/SDL.h"
#include "Camera.hpp"
#include "ParticleSystem.hpp"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_rwops.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_mixer.h"
#include "Texture.hpp"
#include "Types.hpp"
#include "VerletSolver.hpp"
#include "Window.hpp"
#include "libs/Timer/Timer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include "Constants.hpp"
#include "Dot.hpp"

using namespace texture;
using namespace camera;
using namespace verletSolver;

Window g_window;
SDL_Renderer* g_renderer = NULL;

bool init();
void run();
void quit();
void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y);

int main(int argc, char* args[]) {
    if (!init()) {
        return 1;
    }


    run();
    quit();
    return 0;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
        return false;
    }

    if (!g_window.init()) {
        printf("Error initializing window: %s\n", SDL_GetError());
        return false;
    }

    g_renderer = g_window.create_renderer();
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

    if (TTF_Init() == -1) {
        printf("Error while initializing ttf: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void quit() {
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    g_window.free();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void run() {
    bool quit = false;
    SDL_Event event;

    Timer delta_time;

    Solver physics_simulation;
    Vector2<double> rect_position (
        ((float)constants::WINDOW_WIDTH - 100) / 2,
        0
    );
    Vector2<double> rect_size (100, 100);


    physics_simulation.add_rect(rect_position, rect_size);
    
    rect_position = {
        (((float)constants::WINDOW_WIDTH - 100) / 2) + 50, 
        150
    };

    physics_simulation.add_rect(rect_position, rect_size);
    bool simulation_running = false;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (
                event.type == SDL_KEYDOWN 
                && event.key.keysym.sym == SDLK_s
            ) {
                simulation_running = !simulation_running;
            } else if (
                event.type == SDL_MOUSEBUTTONDOWN
                && event.button.button == SDL_BUTTON_LEFT
            ) {
                spawn_rect(
                    physics_simulation, 
                    event.button.x, 
                    event.button.y
                );
            }

            g_window.handle_event(event, g_renderer);
        }


        if (!g_window.is_minimized()) {
            /* calculate and move objects */
            if (simulation_running) {
                physics_simulation.update(delta_time.get_ticks() / 1000.f);    
            }

            delta_time.start();

            SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(g_renderer);

            SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
            physics_simulation.render(g_renderer);

            SDL_RenderPresent(g_renderer);            
        }
    }
}

void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y) {
    Vector2<double> rect_position (
        pos_x,
        pos_y
    );
    int size = 10 + (rand() % 40);
    Vector2<double> rect_size (size, size);


    physics_simulation.add_rect(rect_position, rect_size);
}
