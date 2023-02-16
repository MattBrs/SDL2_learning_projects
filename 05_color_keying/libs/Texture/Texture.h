// class made to handle main texture operations in one place.

#pragma once

#include "SDL2/SDL_render.h"
#include <string>

namespace texture {
    class Texture {
        private:
        SDL_Renderer* mRenderer;
        SDL_Texture* mTexture;
        int mHeigth;
        int mWidth;

        public:
        Texture(SDL_Renderer* &renderer);
        ~Texture();

        bool loadFromPath(std::string path);
        void free();
        void render(int x, int y);

        int getHeigth();
        int getWitdh();
    };
}
