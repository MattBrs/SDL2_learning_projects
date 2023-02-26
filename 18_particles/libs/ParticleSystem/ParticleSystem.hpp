#pragma once

#include "SDL2/SDL_render.h"
#include "Texture.hpp"
#include "Particle.hpp"
#include <string>
#include <vector>

using namespace texture;
using std::string;
using std::vector;

class ParticleSystem {
public:
	ParticleSystem(int particle_life, int particle_count);
	~ParticleSystem();

	bool add_texture_from_path(string path, SDL_Renderer* renderer);
	bool set_shimmer_texture_from_path(string path, SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer, int pos_x, int pos_y);
	bool init();
	void free();
private:
	Texture m_shimmer_texture;
	vector<Texture*> m_particle_textures;
	vector<particle::Particle*> m_particles;
	Texture shimmer_texture;

	int m_max_particle_life = 2;
	int m_max_particle_count = 1;

	bool m_shimmer_texture_set = false;
};