#pragma once

#include <Texture.hpp>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <string>
#include "Constants.hpp"
#include "SDL2/SDL_rect.h"
#include "Utils.hpp"
#include <vector>

class Dot {
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	const int DOT_MAX_VEL = 4;

	Dot();
	Dot(int start_pos_x, int start_pos_y);
	~Dot();

	void handle_event(SDL_Event &event);
	void move(shapes::Circle &other_collider);
	void render(
		SDL_Renderer* renderer,
		int &camera_pos_x, 
		int &camera_pos_y
	);
	bool load_texture(std::string texture_path, SDL_Renderer* renderer);
	void free();

	shapes::Circle& get_collider();
	int get_pos_x();
	int get_pos_y();
private:
	int m_pos_x;
	int m_pos_y;

	int m_vel_x;
	int m_vel_y;

	texture::Texture m_dot_texture;
	shapes::Circle m_collider;

	void handle_movement_input(const SDL_Keycode &keycode, const int &vel);
	void shift_colliders();
};