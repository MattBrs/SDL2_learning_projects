#include "Camera.hpp"
#include "Constants.hpp"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_rect.h"
#include <algorithm>
#include <cstdio>

using namespace camera;

Camera::Camera(int pos_x, int pos_y, int width, int height) {
	m_camera.x = pos_x;
	m_camera.y = pos_y;
	m_camera.w = width;
	m_camera.h = height;
}

Camera::~Camera() {
	// do nothing
}


void Camera::handle_movement_input(const SDL_Keycode &keycode, int vel) {
	switch (keycode) {
	case SDLK_UP:
		m_vel_y -= vel;
		break;
	case SDLK_DOWN:
		m_vel_y += vel;
		break;
	case SDLK_LEFT:
		m_vel_x -= vel;
		break;
	case SDLK_RIGHT:
		m_vel_x += vel;
		break;
	}
}

void Camera::handle_event(SDL_Event &event) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		handle_movement_input(event.key.keysym.sym, MAX_VEL);
	} else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		handle_movement_input(event.key.keysym.sym, -MAX_VEL);
	}
}

void Camera::move(float delta_time) {
	float movement_x = m_vel_x * delta_time;
	m_camera.x += (int)movement_x;

	if (m_camera.x < 0) {
		m_camera.x = 0;
	} else if (m_camera.x > constants::LEVEL_WIDTH - m_camera.w) {
		m_camera.x = constants::LEVEL_WIDTH - m_camera.w;
	}


	float movement_y = m_vel_y * delta_time;
	m_camera.y += (int)movement_y;

	if (m_camera.y < 0) {
		m_camera.y = 0;
	} else if (m_camera.y > constants::LEVEL_HEIGHT - m_camera.h) {
		m_camera.y = constants::LEVEL_HEIGHT - m_camera.h;
	}
}

void Camera::move_with_player_pos(const int &player_x, const int &player_y) {
	if (!m_camera_automovement) {
		return;
	}

	int new_pos_x = m_camera.x;
	if (player_x < m_camera.x + m_camera_padding) {
		new_pos_x = std::max(
			0, 
			m_camera.x - m_camera.w / 2 - m_camera_padding
		);
	} else if (player_x > m_camera.x + m_camera.w - m_camera_padding) {
		new_pos_x = std::min(
			constants::LEVEL_WIDTH - m_camera.w, 
			m_camera.x + m_camera.w / 2 + m_camera_padding
		);
	}
	

	
	int new_pos_y = m_camera.y;
	if (player_y < m_camera.y + m_camera_padding) {
		new_pos_y = std::max(
			0, 
			m_camera.y - m_camera.h / 2 - m_camera_padding
		);
	} else if (player_y > m_camera.y + m_camera.h - m_camera_padding) {
		new_pos_y = std::min(
			constants::LEVEL_HEIGHT - m_camera.h, 
			m_camera.y + m_camera.h / 2 + m_camera_padding
		);
	}
	


	m_camera.x = new_pos_x;
	m_camera.y = new_pos_y;
}

int Camera::get_pos_x() {
	return m_camera.x;
}

int Camera::get_pos_y() {
	return m_camera.y;
}

int Camera::get_width() {
	return m_camera.w;
}

int Camera::get_heigth() {
	return m_camera.h;
}

void Camera::set_camera_x(int pos_x) {
	m_camera.x = pos_x;
}

void Camera::set_camera_y(int pos_y) {
	m_camera.y = pos_y;
}

void Camera::set_camera_automovement(bool automovement_enabled) {
	m_camera_automovement = automovement_enabled;
}

SDL_Rect& Camera::get_camera_rect() {
	return m_camera;
}