#pragma once

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "Texture.hpp"
#include <vector>

namespace particle {
	class Particle {
	public:
		Particle(
			int pos_x, 
			int pos_y, 
			int max_life,
			texture::Texture* texture
		);
		Particle(
			int pos_x, 
			int pos_y, 
			int max_life,
			texture::Texture* texture,
			texture::Texture* shimmer_texture
		);
		~Particle();

		void render(SDL_Renderer* renderer);
		bool is_dead();

	private:
		int m_pos_x;
		int m_pos_y;
		int m_animation_frame;
		int m_max_life;
		texture::Texture* m_texture = NULL;
		texture::Texture* m_shimmer_texture = NULL;
	};
}
