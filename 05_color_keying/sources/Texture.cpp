#include "Texture.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_surface.h"
#include <cstdio>

using namespace texture;

Texture::Texture(SDL_Renderer* &renderer) {
    mRenderer = renderer;
    mTexture = NULL;
    mWidth = 0;
    mHeigth = 0;
}

void Texture::free() {
    // to avoid errors we first check if the texture does point to something
    if (mTexture == NULL) {
        return;
    }

    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeigth = 0;
}

bool Texture::loadFromPath(std::string path) {
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
    newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    
    if (newTexture == NULL) {   
        SDL_FreeSurface(loadedSurface); 
        return false;
    }

    mWidth = loadedSurface->w;
    mHeigth = loadedSurface->h;
    SDL_FreeSurface(loadedSurface); // free surface to avoid memory leaks

    mTexture = newTexture;

    return true;
}

void Texture::render(int x, int y) {
    // set rendering space and render
    SDL_Rect renderQuad = {x, y, mWidth, mHeigth};
    SDL_RenderCopy(mRenderer, mTexture, NULL, &renderQuad);
}

int Texture::getWitdh() {
    return mWidth;
}

int Texture::getHeigth() {
    return mHeigth;
}

Texture::~Texture() {
    // free up memory before deleting object
    free();
}