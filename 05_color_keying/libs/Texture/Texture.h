// class made to handle main texture operations in one place.

#pragma once

#include "SDL2/SDL_render.h"
#include <string>

namespace texture {
    class Texture {
        private:
        SDL_Texture* m_texture;
        int m_heigth;
        int m_width;

        public:
        Texture();
        ~Texture();

        bool load_from_path(std::string path, SDL_Renderer* renderer);
        void render(int x, int y, SDL_Renderer* renderer);
        void free();

        int get_heigth();
        int get_witdh();
    };
}
