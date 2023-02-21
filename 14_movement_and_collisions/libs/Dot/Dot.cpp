#include "Dot.hpp"
#include "SDL2/SDL_render.h"
#include "Constants.hpp"
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

	// here we make colliders specifically made for the used texture
	// a more readable and maybe correct alternative should be create less
	// colliders, more approximated.

	m_colliders.resize(3);
	m_colliders[0].w = 10;
	m_colliders[0].h = DOT_HEIGHT / 3;

	m_colliders[1].w = DOT_WIDTH;
	m_colliders[1].h = DOT_HEIGHT / 3;

	m_colliders[2].w = 10;
	m_colliders[2].h = DOT_HEIGHT / 3;

	/*
	m_colliders.resize(11);

	m_colliders[0].w = 6;
	m_colliders[0].h = 1;

	m_colliders[1].w = 10;
	m_colliders[1].h = 1;
	
	m_colliders[2].w = 14;
	m_colliders[2].h = 1;
	
	m_colliders[3].w = 16;
	m_colliders[3].h = 1;
	
	m_colliders[4].w = 18;
	m_colliders[4].h = 1;
	
	m_colliders[5].w = 20;
	m_colliders[5].h = 1;
	
	m_colliders[6].w = 18;
	m_colliders[6].h = 1;
	
	m_colliders[7].w = 16;
	m_colliders[7].h = 1;
	
	m_colliders[8].w = 14;
	m_colliders[8].h = 1;
	
	m_colliders[9].w = 10;
	m_colliders[9].h = 1;
	
	m_colliders[10].w = 6;
	m_colliders[10].h = 1;
	*/

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

void Dot::move(std::vector<SDL_Rect> &other_colliders) {
	// after we move the dot (both the first time or when we return
	// to previous position because of conditions), we move the colliders
	m_pos_x += m_vel_x;
	shift_colliders();
	

	if (m_pos_x < 0) {
		m_pos_x = constants::WINDOW_WIDTH - DOT_WIDTH;
		shift_colliders();
	} else if (m_pos_x + DOT_WIDTH > constants::WINDOW_WIDTH) {
		m_pos_x = 0;
		shift_colliders();
	} 

	if (collisions::check_collision(m_colliders, other_colliders)) {
		m_pos_x -= m_vel_x;
		shift_colliders();
	}

	m_pos_y += m_vel_y;
	shift_colliders();

	if (
		m_pos_y < 0 
		|| m_pos_y + DOT_HEIGHT > constants::WINDOW_HEIGHT
		|| collisions::check_collision(m_colliders, other_colliders)
	) {
		m_pos_y -= m_vel_y;
		shift_colliders();
	}
}

void Dot::shift_colliders() {
	int row_offset = 0;
	for (int set = 0; set < m_colliders.size(); ++set) {
		m_colliders[set].x = m_pos_x + (DOT_WIDTH - m_colliders[set].w) / 2;
		m_colliders[set].y = m_pos_y + row_offset;

		row_offset += m_colliders[set].h;
	}

}

void Dot::render(SDL_Renderer* renderer) {
	m_dot_texture.render(m_pos_x, m_pos_y, renderer);
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

std::vector<SDL_Rect>& Dot::get_colliders() {
	return m_colliders;
}