#include "Dot.hpp"
#include "SDL2/SDL_render.h"
#include "Constants.hpp"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <cstdio>

Dot::Dot() {
	m_pos_x = constants::WINDOW_WIDTH / 2;
	m_pos_y = constants::WINDOW_HEIGHT / 2;

	m_vel_x = 0;
	m_vel_y = 0;
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

void Dot::move() {
	m_pos_x += m_vel_x;
	m_pos_y += m_vel_y;

	if (m_pos_x < 0) {
		m_pos_x = constants::WINDOW_WIDTH - DOT_WIDTH;
	} else if (m_pos_x + DOT_WIDTH > constants::WINDOW_WIDTH) {
		m_pos_x = 0;
	}

	if (m_pos_y < 0 || m_pos_y + DOT_HEIGHT > constants::WINDOW_HEIGHT) {
		m_pos_y -= m_vel_y;
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