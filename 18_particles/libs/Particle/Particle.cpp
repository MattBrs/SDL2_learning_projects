#include "Particle.hpp"
#include "Texture.hpp"
#include <cstdlib>
#include <iterator>
#include <vector>

using namespace particle;

Particle::Particle(
	int pos_x, 
	int pos_y, 
	int max_life,
	texture::Texture* texture
) :Particle(pos_x, pos_y, max_life, texture, NULL) {}

Particle::Particle(
	int pos_x, 
	int pos_y, 
	int max_life,
	texture::Texture* texture,
	texture::Texture* shimmer_texture
) {
	m_pos_x = pos_x - 5 + rand() % 25;
	m_pos_y = pos_y - 5 + rand() % 25;

	m_max_life = max_life;
	m_texture = texture;
	m_shimmer_texture = texture;
	m_animation_frame = rand() % m_max_life / 2;
}


Particle::~Particle() {
	// do nothing
}

void Particle::render(SDL_Renderer *renderer) {
	m_texture->render(m_pos_x, m_pos_y, renderer);

	if (m_shimmer_texture != NULL) {
		m_shimmer_texture->render(m_pos_x, m_pos_y, renderer);
	}

	m_animation_frame++;
}

bool Particle::is_dead() {
	return m_animation_frame > m_max_life;
}