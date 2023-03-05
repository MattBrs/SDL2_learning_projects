#include "Dot.hpp"
#include "SDL2/SDL_render.h"
#include "Constants.hpp"
#include "Particle.hpp"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rect.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <cstdio>
#include <vector>
#include "SDL2/SDL_scancode.h"
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

void Dot::handle_movement(const Uint8* current_key_states) {
	bool accelerating_x = false;
	bool accelerating_y = false;

	if (current_key_states[SDL_SCANCODE_W]) {
		m_vel_y -= MAX_ACCELERATION;
		accelerating_y = true;
	} 

	if (current_key_states[SDL_SCANCODE_S]) {
		m_vel_y += MAX_ACCELERATION;
		accelerating_y = true;
	} 

	if (current_key_states[SDL_SCANCODE_A]) {
		m_vel_x -= MAX_ACCELERATION;
		accelerating_x = true;
	} 
	if (current_key_states[SDL_SCANCODE_D]) {
		m_vel_x += MAX_ACCELERATION;
		accelerating_x = true;
	}

	m_is_accelerating_x = accelerating_x;
	m_is_accelerating_y = accelerating_y;

	if (m_vel_x > DOT_MAX_VEL || m_vel_x < -DOT_MAX_VEL) {
		m_vel_x = ((m_vel_x > 0) - (m_vel_x < 0)) * DOT_MAX_VEL;
	}

	if (m_vel_y > DOT_MAX_VEL || m_vel_y < -DOT_MAX_VEL) {
		m_vel_y = ((m_vel_y > 0) - (m_vel_y < 0)) *  DOT_MAX_VEL;
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
		m_vel_x = 0;
		shift_colliders();
	}

	if (
		collisions::check_collision(m_collider, other_collider) 
	) {
		m_pos_x -= movement_x;
		m_vel_x = 0;
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
		m_vel_y = 0;
		shift_colliders();
	}

	if (collisions::check_collision(m_collider, other_collider)) {
		m_pos_y -= movement_y;
		m_vel_y = 0;
		shift_colliders();
	}

	// handle if the curr vel is positive or negative 
	// and subtract deceleration speed.
	// if is accelerating, skip friction deceleration
	if (!m_is_accelerating_x && m_vel_x > 0) {
		m_vel_x = std::max(
			(int) (m_vel_x - (MAX_DECELERATION * time_step * 100)),
			0
		);
	} else if (!m_is_accelerating_x && m_vel_x < 0) {
		m_vel_x = std::min(
			(int) (m_vel_x - (-MAX_DECELERATION * time_step * 100)),
			0
		);
	}

	
	if (!m_is_accelerating_y && m_vel_y > 0) {
		m_vel_y = std::max(
			(int) (m_vel_y - (MAX_DECELERATION * time_step * 100)),
			0
		);
	} else if (!m_is_accelerating_y && m_vel_y < 0) {
		m_vel_y = std::min(
			(int) (m_vel_y - (-MAX_DECELERATION * time_step * 100)),
			0
		);
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
	const int &camera_pos_x, 
	const int &camera_pos_y
) {
	m_dot_texture.render(
		(int)m_pos_x - (int)camera_pos_x, 
		(int)m_pos_y - (int)camera_pos_y, 
		renderer
	);

	if (m_particle_system != NULL) {
	 	m_particle_system->set_pos(
	 		(int)m_pos_x, 
	 		(int)m_pos_y
 		);

		m_particle_system->render(
			renderer, 
			(int)camera_pos_x, 
			(int)camera_pos_y
		);
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