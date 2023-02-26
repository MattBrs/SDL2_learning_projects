#include "Dot.hpp"
#include "SDL2/SDL_render.h"
#include "Constants.hpp"
#include "Particle.hpp"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rect.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <cstdio>
#include <vector>
#include "Utils.hpp"

Dot::Dot(): Dot(constants::WINDOW_WIDTH / 2, constants::WINDOW_HEIGHT / 2) {}

Dot::Dot(int start_pos_x, int start_pos_y) {
	m_pos_x = start_pos_x;
	m_pos_y = start_pos_y;

	m_vel_x = 0;
	m_vel_y = 0;

	m_collider.radius = ((float)DOT_WIDTH) / 2;

	m_particle_system = NULL;
	// initialize colliders relative to position
	shift_colliders();
}

Dot::~Dot() {
	free();
}

void Dot::handle_movement_input(const SDL_Keycode &keycode, const int &vel) {
	switch (keycode) {
		case SDLK_w:
			m_vel_y -= vel;
			break;
		case SDLK_a:
			m_vel_x -= vel;
			break;
		case SDLK_s:
			m_vel_y += vel;
			break;
		case SDLK_d:
			m_vel_x += vel;
			break;
	}
}

void Dot::handle_event(SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		handle_movement_input(event.key.keysym.sym, DOT_MAX_VEL);
	} else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		handle_movement_input(event.key.keysym.sym, -DOT_MAX_VEL);
	}
}

void Dot::move(
	shapes::Circle &other_collider, 
	float time_step
) {
	// after we move the dot (both the first time or when we return
	// to previous position because of conditions), we move the colliders
	float movement_x = m_vel_x * time_step;
	m_pos_x += movement_x;
	shift_colliders();

	
	if (m_pos_x < 0 || m_pos_x > constants::LEVEL_WIDTH - DOT_WIDTH) {
		m_pos_x = m_pos_x < 0 ? 0 : constants::LEVEL_WIDTH - DOT_WIDTH;
		// m_pos_x -= movement_x;
		shift_colliders();
	}

	if (
		collisions::check_collision(m_collider, other_collider) 
	) {
		m_pos_x -= movement_x;
		shift_colliders();
	}

	float movement_y = m_vel_y * time_step;
	m_pos_y += movement_y;
	shift_colliders();
	
	if (
		m_pos_y < 0 
		|| m_pos_y > constants::LEVEL_HEIGHT - DOT_HEIGHT
	) {
		m_pos_y = m_pos_y < 0 ? 0 : constants::LEVEL_HEIGHT - DOT_HEIGHT;
		shift_colliders();
	}

	if (collisions::check_collision(m_collider, other_collider)) {
		m_pos_y -= movement_y;
		shift_colliders();
	}
	
}

void Dot::move( 
	float time_step
) {
	// after we move the dot (both the first time or when we return
	// to previous position because of conditions), we move the colliders
	float movement_x = m_vel_x * time_step;
	m_pos_x += movement_x;
	shift_colliders();

	
	if (m_pos_x < 0 || m_pos_x > constants::LEVEL_WIDTH - DOT_WIDTH) {
		m_pos_x = m_pos_x < 0 ? 0 : constants::LEVEL_WIDTH - DOT_WIDTH;
		// m_pos_x -= movement_x;
		shift_colliders();
	}

	float movement_y = m_vel_y * time_step;
	m_pos_y += movement_y;
	shift_colliders();
	
	if (
		m_pos_y < 0 
		|| m_pos_y > constants::LEVEL_HEIGHT - DOT_HEIGHT
	) {
		m_pos_y = m_pos_y < 0 ? 0 : constants::LEVEL_HEIGHT - DOT_HEIGHT;
		shift_colliders();
	}
 
}

void Dot::shift_colliders() {
	m_collider.pos_x = m_pos_x + ((float)DOT_WIDTH / 2);
	m_collider.pos_y = m_pos_y + ((float)DOT_HEIGHT / 2);
}

void Dot::render(
	SDL_Renderer* renderer, 
	int &camera_pos_x, 
	int &camera_pos_y
) {
	m_dot_texture.render(
		(int)m_pos_x - (int)camera_pos_x, 
		(int)m_pos_y - (int)camera_pos_y, 
		renderer
	);

	if (m_particle_system != NULL) {
		m_particle_system->render(renderer, m_pos_x, m_pos_y);
	}
}

bool Dot::load_texture(std::string texture_path, SDL_Renderer* renderer) {
	if (!m_dot_texture.load_from_path(texture_path, renderer)) {
		printf("Error loading dot texture! %s\n", IMG_GetError());
		return false;
	}

	return true;
}

void Dot::free() {
	m_dot_texture.free();
}

shapes::Circle& Dot::get_collider() {
	return m_collider;
}

int Dot::get_pos_x() {
	return m_pos_x;
}

int Dot::get_pos_y() {
	return m_pos_y;
}

void Dot::set_pos_x(const int &new_pos_x) {
	m_pos_x = new_pos_x;
}

void Dot::set_pos_y(const int &new_pos_y) {
	m_pos_y = new_pos_y;
}

void Dot::set_particle_system(ParticleSystem *particle_system) {
	m_particle_system = particle_system;
}