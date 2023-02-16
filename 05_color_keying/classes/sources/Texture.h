// class made to handle main texture operations in one place.

#pragma once

#include "SDL2/SDL_render.h"
#include <string>

namespace texture {
    class Texture {
        private:
        SDL_Renderer* mRenderer;
        SDL_Texture* m_texture;
        int m_heigth;
        int m_width;

        public:
        Texture(SDL_Renderer* &renderer);
        ~Texture();

        bool loadFromPath(std::string path);
        void free();
        void render(int x, int y);

        int get_heigth();
        int get_witdh();
    };
}
