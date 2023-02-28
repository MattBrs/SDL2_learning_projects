#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "SDL2/SDL_render.h"
#include "Texture.hpp"
#include <cstdio>
#include <cstdlib>

ParticleSystem::ParticleSystem(int particle_life, int particle_count) {
	m_max_particle_count = particle_count;
	m_max_particle_life = particle_life;
}

bool ParticleSystem::init() {
	if (m_particle_textures.size() == 0) {
		printf("Cannot init particleSystem: no texture available");
		return false;
	}

	m_particles.resize(m_max_particle_count);
	for (int i = 0; i < m_particles.size(); ++i) {
		m_particles[i] = new particle::Particle(
			m_pos_x, 
			m_pos_y, 
			m_max_particle_life,
			m_particle_textures[rand() % m_particle_textures.size()],
			m_shimmer_texture_set ? &m_shimmer_texture:NULL
		);
	}

	return true;
}

ParticleSystem::~ParticleSystem() {
	free();
}

void ParticleSystem::free() {
	for (int i = 0; i < m_particles.size(); ++i) {
		delete m_particles[i];
	}	

	for (int i = 0; i < m_particle_textures.size(); ++i) {
		m_particle_textures[i]->free();
		delete m_particle_textures[i];
	}

	m_shimmer_texture.free();
}

bool ParticleSystem::add_texture_from_path(
	string path, 
	SDL_Renderer* renderer
) {
	Texture* new_texture = new Texture();

	if (!new_texture->load_from_path(path, renderer)) {
		return false;
	}

	new_texture->set_alpha(180);
	m_particle_textures.push_back(new_texture);

	return true;
}

bool ParticleSystem::set_shimmer_texture_from_path(
	string path, 
	SDL_Renderer* renderer
) {
	if (!m_shimmer_texture.load_from_path(path, renderer)) {
		m_shimmer_texture_set = false;
		return false;
	}

	m_shimmer_texture.set_alpha(180);
	m_shimmer_texture_set = true;
	return true;	
}

void ParticleSystem::render(
	SDL_Renderer *renderer,
	const int &camera_x,
	const int &camera_y
) {
	for (int i = 0; i < m_particles.size(); ++i) {
		if (m_particles[i]->is_dead()) {
			delete m_particles[i];
			m_particles[i] = new particle::Particle(
				(int)m_pos_x, 
				(int)m_pos_y, 
				m_max_particle_life,
				m_particle_textures[rand() % m_particle_textures.size()],
				(
					m_shimmer_texture_set
				) ? &m_shimmer_texture:NULL
			);
		}
	}

	for (int i = 0; i < m_particles.size(); ++i) {
		m_particles[i]->render(renderer, camera_x, camera_y);
	}
}

void ParticleSystem::set_pos(float pos_x, float pos_y) {
	m_pos_x = pos_x;
	m_pos_y = pos_y;
}
