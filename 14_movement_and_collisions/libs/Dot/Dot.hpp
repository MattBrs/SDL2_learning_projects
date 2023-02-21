#pragma once

#include <Texture.hpp>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <string>
#include "Constants.hpp"

class Dot {
public:
	Dot();
	~Dot();

	void handle_event(SDL_Event &event);
	void move();
	void render(SDL_Renderer* renderer);
	bool load_texture(std::string texture_path, SDL_Renderer* renderer);
	void free();
private:
	const int DOT_WIDTH = 20;
	const int DOT_HEIGHT = 20;
	const int DOT_MAX_VEL = 10;

	int m_pos_x;
	int m_pos_y;

	int m_vel_x;
	int m_vel_y;

	texture::Texture m_dot_texture;

	void handle_movement_input(const SDL_Keycode &keycode, const int &vel);
};