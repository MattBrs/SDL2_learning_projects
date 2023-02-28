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
	ParticleSystem(int particle_life = 15, int particle_count = 30);
	~ParticleSystem();

	bool add_texture_from_path(string path, SDL_Renderer* renderer);
	bool set_shimmer_texture_from_path(string path, SDL_Renderer* renderer);
	void render(
		SDL_Renderer* renderer, 
		const int &camera_x, 
		const int &camera_Y
	);
	bool init();
	void free();

	void set_pos(float pos_x, float pos_y);
private:
	Texture m_shimmer_texture;
	vector<Texture*> m_particle_textures;
	vector<particle::Particle*> m_particles;
	Texture shimmer_texture;

	int m_max_particle_life = 15;
	int m_max_particle_count = 30;

	float m_pos_x;
	float m_pos_y;

	bool m_shimmer_texture_set = false;
};