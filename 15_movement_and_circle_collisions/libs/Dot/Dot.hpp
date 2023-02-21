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
	Dot();
	Dot(int start_pos_x, int start_pos_y);
	~Dot();

	void handle_event(SDL_Event &event);
	void move(shapes::Circle &other_collider, SDL_Rect &wall_collider);
	void render(SDL_Renderer* renderer);
	bool load_texture(std::string texture_path, SDL_Renderer* renderer);
	void free();

	shapes::Circle& get_collider();
private:
	const int DOT_WIDTH = 20;
	const int DOT_HEIGHT = 20;
	const int DOT_MAX_VEL = 4;

	int m_pos_x;
	int m_pos_y;

	int m_vel_x;
	int m_vel_y;

	texture::Texture m_dot_texture;
	shapes::Circle m_collider;

	void handle_movement_input(const SDL_Keycode &keycode, const int &vel);
	void shift_colliders();
};