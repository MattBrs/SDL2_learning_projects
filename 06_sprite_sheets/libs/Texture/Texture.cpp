#include "Texture.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include <cstdio>

using namespace texture;

Texture::Texture() {
    m_texture = NULL;
    m_width = 0;
    m_heigth = 0;
}

void Texture::free() {
    // to avoid errors we first check if the texture does point to something
    if (m_texture == NULL) {
        return;
    }

    SDL_DestroyTexture(m_texture);
    m_texture = NULL;
    m_width = 0;
    m_heigth = 0;
}

bool Texture::load_from_path(std::string path, SDL_Renderer* renderer) {
    // free previous stored texture to avoid memory leaks, the load surface from image
    free();
    SDL_Texture* newTexture;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        printf("Unable to load image from: %s. Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    // define which color will be treated as trasparent (setColorKey) to handle image transparency
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xff, 0xff));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    
    if (newTexture == NULL) {   
        SDL_FreeSurface(loadedSurface); 
        printf("Error while loading texture: %s\n", SDL_GetError());
        return false;
    }

    m_width = loadedSurface->w;
    m_heigth = loadedSurface->h;
    SDL_FreeSurface(loadedSurface); // free surface to avoid memory leaks

    m_texture = newTexture;

    return true;
}

// we use clip to define a portion of the texture we want to render. 
// If null, render the whole texture. (on spritesheets we want only some parts)
void Texture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip) {
    SDL_Rect renderQuad = {x, y, m_width, m_heigth};
    if (clip != NULL) {
        // change the dimensions to the clip dimensions to avoid problems 
        // (since we are selecting a portion of the texture smaller than the real size)
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // we set "clip" as the third parameter to specify the src_quad
    // which means the specific portion of the texture we want to render
    SDL_RenderCopy(renderer, m_texture, clip, &renderQuad);
}

void Texture::set_color(Uint8 red, Uint8 green, Uint8 blue) {
    // morph the base colors of the texture
    SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void Texture::set_blend_mode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(m_texture, blending);
}

void Texture::set_alpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(m_texture, alpha);
}

int Texture::get_witdh() {
    return m_width;
}

int Texture::get_heigth() {
    return m_heigth;
}

Texture::~Texture() {
    // free up memory before deleting object
    free();
}