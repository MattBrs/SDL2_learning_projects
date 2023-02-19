#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_video.h"
#include "SDL2/SDL_mixer.h"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <stdio.h>

using namespace texture;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 720;

SDL_Renderer* g_renderer = NULL;

bool init(SDL_Window* &window);
void run(
    Texture &texture,
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
);
void quit(
    SDL_Window* &window, 
    Texture &texture,
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
);
bool load_media(
    Texture &texture, 
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
);
void handle_music(Mix_Music* &music);

int main(int argc, char* args[]) {
    SDL_Window* window;
    Texture loaded_texture;
    Mix_Music* music = NULL;
    Mix_Chunk* scratch_chunck = NULL;
    Mix_Chunk* high_chunck = NULL;
    Mix_Chunk* low_chunck = NULL;
    Mix_Chunk* medium_chunck = NULL;

    if (!init(window)) {
        return 1;
    }

    if (!load_media(
        loaded_texture,
        music,
        scratch_chunck,
        high_chunck,
        medium_chunck,
        low_chunck
        )
    ) {
        return 1;
    }

    run(
        loaded_texture, 
        music, 
        scratch_chunck, 
        high_chunck, 
        medium_chunck, 
        low_chunck
    );
    quit(
        window, 
        loaded_texture,
        music,
        scratch_chunck,
        high_chunck,
        medium_chunck,
        low_chunck
    );
    return 0;
}

bool init(SDL_Window* &window) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Error initializing sdl: %s\n", SDL_GetError());
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) {
        printf("Error initializing audio: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

void quit(
    SDL_Window* &window, 
    Texture &texture,
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
) {
    Mix_FreeChunk(scratch_chunck);
    Mix_FreeChunk(high_chunck);
    Mix_FreeChunk(medium_chunck);
    Mix_FreeChunk(low_chunck);
    scratch_chunck = NULL;
    high_chunck = NULL;
    medium_chunck = NULL;
    low_chunck = NULL;

    Mix_FreeMusic(music);
    music = NULL;

    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    texture.free();

    IMG_Quit();
    SDL_Quit();
}

bool load_media(
    Texture &texture, 
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
) {
    if (!texture.load_from_path("textures/prompt.png", g_renderer)) {
        printf("Error loading prompt image\n");
        return false;
    }

    music = Mix_LoadMUS("audio/beat.wav");
    if (music == NULL) {
        printf("Error loading music: %s\n", Mix_GetError());
        return false;
    }
    
    scratch_chunck = Mix_LoadWAV("audio/scratch.wav");
    if (scratch_chunck == NULL) {
        printf("Error loading chunk: %s\n", Mix_GetError());   
        return false;
    }

    high_chunck = Mix_LoadWAV("audio/high.wav");
    if (high_chunck == NULL) {
        printf("Error loading chunk: %s\n", Mix_GetError());
        return false;
    }

    medium_chunck = Mix_LoadWAV("audio/medium.wav");
    if (medium_chunck == NULL) {
        printf("Error loading chunk: %s\n", Mix_GetError());
        return false;
    }

    low_chunck = Mix_LoadWAV("audio/low.wav");
    if (low_chunck == NULL) {
        printf("Error loading chunk: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

void run(
    Texture &texture,
    Mix_Music* &music,
    Mix_Chunk* &scratch_chunck,
    Mix_Chunk* &high_chunck,
    Mix_Chunk* &medium_chunck,
    Mix_Chunk* &low_chunck
) {
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        Mix_PlayChannel(-1, scratch_chunck, 0);
                        break;
                    case SDLK_2:
                        Mix_PlayChannel(-1, high_chunck, 0);
                        break;
                    case SDLK_3:
                        Mix_PlayChannel(-1, medium_chunck, 0);
                        break;
                    case SDLK_4:
                        Mix_PlayChannel(-1, low_chunck, 0);
                        break;
                    case SDLK_9:
                        handle_music(music);
                        break;
                    case SDLK_0:
                        Mix_HaltMusic();
                        break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(g_renderer);
        texture.render(0, 0, g_renderer);
        SDL_RenderPresent(g_renderer);
        
    }
}

void handle_music(Mix_Music* &music) {
    if (Mix_PlayingMusic() == 0) {
        // music is not playing
        Mix_PlayMusic(music, -1);
    } else {
        if (Mix_PausedMusic() == 1) {
            // music is paused
            Mix_ResumeMusic();
        } else {
            // music is playing
            Mix_PauseMusic();
        }
    }
}
